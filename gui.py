import tkinter as tk
import subprocess

# Path to your compiled C++ EXE
EXE_PATH = r"C:\Users\HP\OneDrive\Desktop\naive bayes\build\spam_detector.exe"

root = tk.Tk()
root.title("Naive Bayes Threat Detector")
root.geometry("600x500")


# ---------------- UI ELEMENTS ----------------

label = tk.Label(root, text="Enter a message:", font=("Arial", 12))
label.pack(pady=10)

input_box = tk.Text(root, height=6, width=60)
input_box.pack(pady=5)

def classify_message():
    msg = input_box.get("1.0", "end").strip()
    if not msg:
        result_box.delete("1.0", "end")
        result_box.insert("1.0", "Empty input.")
        return

    try:
        process = subprocess.run(
            [EXE_PATH],
            input=msg,                 # <-- FIXED
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True                  # handles automatic encoding/decoding
        )

        output = process.stdout.strip()

        result_box.delete("1.0", "end")
        result_box.insert("1.0", output)

    except Exception as e:
        result_box.delete("1.0", "end")
        result_box.insert("1.0", f"Error: {e}")



button = tk.Button(root, text="Classify", command=classify_message, font=("Arial", 12))
button.pack(pady=10)

result_label = tk.Label(root, text="Output:", font=("Arial", 12, "bold"))
result_label.pack()

# Multi-line text box for full output
result_box = tk.Text(root, height=12, width=60)
result_box.pack(pady=10)


root.mainloop()
