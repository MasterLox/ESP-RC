import tkinter as tk
from tkinter import ttk, messagebox
import os

# Default values for pins
default_servo_pin_zwei = 21
default_servo_pin = 22
default_servo_pin_eins = 23

# Function to handle the Save button click
def save_pins():
    servo_pin_zwei = servo_pin_zwei_entry.get()
    servo_pin = servo_pin_entry.get()
    servo_pin_eins = servo_pin_eins_entry.get()

    # Set pins to default values if no input is provided
    servo_pin_zwei = int(servo_pin_zwei) if servo_pin_zwei else default_servo_pin_zwei
    servo_pin = int(servo_pin) if servo_pin else default_servo_pin
    servo_pin_eins = int(servo_pin_eins) if servo_pin_eins else default_servo_pin_eins

    input_file_path = "espnow-rx.ino"  # Update with the correct input file path and escape the backslashes
    output_file_path = "esprc/src/espnow-rx.ino"  # Update with the desired output file path and escape the backslashes

    with open(input_file_path, "r") as input_file:
        code = input_file.read()

    modified_code = code.replace("byte servoPinZwei = 21;", f"byte servoPinZwei = {servo_pin_zwei};")
    modified_code = modified_code.replace("byte servoPin = 22;", f"byte servoPin = {servo_pin};")
    modified_code = modified_code.replace("byte servoPinEins = 23;", f"byte servoPinEins = {servo_pin_eins};")

    with open(output_file_path, "w") as output_file:
        output_file.write(modified_code)

    messagebox.showinfo("Success", "Modified code saved successfully!")

# Function to handle the Build and Flash button click
def build_and_flash():
    os.system("compile.py")  # Replace "path_to_build_and_flash_script.py" with the actual path to your Python script

# Create the main window
window = tk.Tk()
window.title("ESPNOW-RX Flasher")

# Set the window size and make it resizable
window.geometry("600x300")
window.resizable(True, True)

# Configure colors for ExpressLRS theme
window.configure(bg="#1f1f1f")
style = ttk.Style()
style.theme_create("ExpressLRSTheme", settings={
    "TLabel": {"configure": {"background": "#1f1f1f", "foreground": "#dcdcdc", "font": ('Arial', 12)}},
    "TEntry": {"configure": {"fieldbackground": "#242424", "foreground": "#dcdcdc", "font": ('Arial', 12)}},
    "TButton": {"configure": {"background": "#0078d7", "foreground": "#dcdcdc", "font": ('Arial', 12)}},
})
style.theme_use("ExpressLRSTheme")

# Create labels and entry fields for pins
servo_pin_zwei_label = ttk.Label(window, text="Servo Pin Zwei:")
servo_pin_zwei_entry = ttk.Entry(window, font=('Arial', 12))
servo_pin_zwei_entry.insert(0, default_servo_pin_zwei)

servo_pin_label = ttk.Label(window, text="Servo Pin:")
servo_pin_entry = ttk.Entry(window, font=('Arial', 12))
servo_pin_entry.insert(0, default_servo_pin)

servo_pin_eins_label = ttk.Label(window, text="Servo Pin Eins:")
servo_pin_eins_entry = ttk.Entry(window, font=('Arial', 12))
servo_pin_eins_entry.insert(0, default_servo_pin_eins)

# Create the Save button
save_button = ttk.Button(window, text="Save Pins", command=save_pins, width=15)

# Create the Build and Flash button
build_flash_button = ttk.Button(window, text="Build and Flash", command=build_and_flash, width=15)

# Positioning the widgets using grid layout
servo_pin_zwei_label.grid(row=0, column=0, padx=10, pady=10, sticky=tk.W)
servo_pin_zwei_entry.grid(row=0, column=1, padx=10, pady=10)

servo_pin_label.grid(row=1, column=0, padx=10, pady=10, sticky=tk.W)
servo_pin_entry.grid(row=1, column=1, padx=10, pady=10)

servo_pin_eins_label.grid(row=2, column=0, padx=10, pady=10, sticky=tk.W)
servo_pin_eins_entry.grid(row=2, column=1, padx=10, pady=10)

save_button.grid(row=3, column=0, padx=10, pady=10)
build_flash_button.grid(row=3, column=1, padx=10, pady=10)

# Start the main event loop
window.mainloop()
