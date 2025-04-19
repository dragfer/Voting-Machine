import csv

CSV_FILE = "voters.csv"

def load_voters():
    with open(CSV_FILE, newline='') as f:
        reader = csv.DictReader(f)
        return list(reader)

def save_voters(voters):
    with open(CSV_FILE, "w", newline='') as f:
        writer = csv.DictWriter(f, fieldnames=["id", "name", "used", "party"])
        writer.writeheader()
        for voter in voters:
            writer.writerow(voter)

def mark_voter_used(voter_id, party=None):
    voters = load_voters()
    for voter in voters:
        if voter["id"] == voter_id:
            voter["used"] = "True"  # Mark the voter as used
            if party:
                voter["party"] = party  # Record the party they voted for
            save_voters(voters)  # Save the updated list back to the file
            return

import csv
import os

VOTER_FILE = "voters.csv"

def reset_voters():
    if not os.path.exists(VOTER_FILE):
        print("CSV file not found!")
        return

    with open(VOTER_FILE, "r") as f:
        reader = csv.DictReader(f)
        voters = list(reader)

    if not voters:
        print("No voter data found in file!")
        return

    for voter in voters:
        voter["used"] = "False"
        voter["party"] = ""

    with open(VOTER_FILE, "w", newline='') as f:
        fieldnames = ["id", "name", "used", "party"]  # include all fields!
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(voters)
