import tkinter as tk
from tkinter import messagebox
import serial
import threading
from utils import load_voters, mark_voter_used, reset_voters

ser = serial.Serial("COM5", 9600, timeout=1)  # Change COM port if needed

approved_voter = {"id": None}

def approve_voter():
    voter_id = entry.get().strip()
    voters = load_voters()
    for voter in voters:
        if voter["id"] == voter_id:
            if voter["used"] == "True":
                status_label.config(text="❌ Already Voted", fg="red")
                entry.delete(0, tk.END)  # Clear input field
                return  # Stop here if already voted
            else:
                ser.write(b"APPROVED\n")
                status_label.config(text="✅ Approved", fg="green")
                approved_voter["id"] = voter_id
                mark_voter_used(voter_id)  # Mark this voter as voted
                break
    else:
        status_label.config(text="❌ Not Found", fg="red")
        entry.delete(0, tk.END)

    # After 2 seconds, reset the form for the next user
    app.after(2000, clear_input)

def clear_input():
    entry.delete(0, tk.END)
    status_label.config()

def read_serial():
    while True:
        try:
            line = ser.readline().decode().strip()
            if line.startswith("VOTED:"):
                party = line.split(":")[1]
                if approved_voter["id"]:
                    mark_voter_used(approved_voter["id"], party)
                    approved_voter["id"] = None
            elif line == "RESET":
                reset_voters()
        except:
            pass

app = tk.Tk()
app.title("Electronic Voting System")
app.geometry("300x200")
app.configure(bg="white")

tk.Label(app, text="Enter Voter ID:", bg="white").pack(pady=5)
entry = tk.Entry(app)
entry.pack(pady=5)
entry.focus()

tk.Button(app, text="Approve", command=approve_voter).pack(pady=5)
status_label = tk.Label(app)
status_label.pack(pady=10)

# Vote History display removed

threading.Thread(target=read_serial, daemon=True).start()
app.mainloop()
