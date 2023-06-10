import tkinter as tk
from tkinter import ttk, messagebox
import os
import re

# Default values for pins
default_ppm_pin = 2
default_b1_pin = 17
default_b2_pin = 16
default_ss = 18
default_rst = 14
default_dio0 = 26
default_clock = 22
default_data = 21
default_reset = "U8X8_PIN_NONE"
default_mac_address = "00:1b:63:84:45:e6"

# Function to handle the Save button click
def save_pins():
    ppm_pin = ppm_pin_entry.get()
    b1_pin = b1_pin_entry.get()
    b2_pin = b2_pin_entry.get()
    ss_pin = ss_pin_entry.get()
    rst_pin = rst_pin_entry.get()
    dio0_pin = dio0_pin_entry.get()
    clock_pin = clock_pin_entry.get()
    data_pin = data_pin_entry.get()
    reset_pin = reset_pin_entry.get()
    mac_address = mac_address_entry.get()

    # Set pins to default values if no input is provided
    ppmPin = int(ppm_pin) if ppm_pin else default_ppm_pin
    b1 = int(b1_pin) if b1_pin else default_b1_pin
    b2 = int(b2_pin) if b2_pin else default_b2_pin
    ss = int(ss_pin) if ss_pin else default_ss
    rst = int(rst_pin) if rst_pin else default_rst
    dio0 = int(dio0_pin) if dio0_pin else default_dio0
    clock = int(clock_pin) if clock_pin else default_clock
    data = int(data_pin) if data_pin else default_data
    reset = reset_pin if reset_pin else default_reset

    # Convert MAC address to a format compatible with the Arduino code
    mac_address = mac_address.replace(":", ", 0x")

    input_file_path = "dual-esp-rc.ino"  # Update with the correct input file path and escape the backslashes
    output_file_path = "esprc/src/dual-esp-rc.ino"  # Update with the desired output file path and escape the backslashes

    with open(input_file_path, "r") as input_file:
        code = input_file.read()

    modified_code = code.replace("int ppmPin = 2;", f"int ppmPin = {ppmPin};")
    modified_code = modified_code.replace("int b1 = 17;", f"int b1 = {b1};")
    modified_code = modified_code.replace("int b2 = 16;", f"int b2 = {b2};")
    modified_code = modified_code.replace("#define ss 18", f"#define ss {ss}")
    modified_code = modified_code.replace("#define rst 14", f"#define rst {rst}")
    modified_code = modified_code.replace("#define dio0 26", f"#define dio0 {dio0}")
    modified_code = modified_code.replace("/* clock=*/ 22", f"/* clock=*/ {clock}")
    modified_code = modified_code.replace("/* data=*/ 21", f"/* data=*/ {data}")
    modified_code = modified_code.replace("/* reset=*/ U8X8_PIN_NONE", f"/* reset=*/ {reset}")

    # Find the line that defines the ESP-Rx-MacAddress
    mac_address_line = re.search(r"uint8_t broadcastAddress\[\] = \{.*?\};", modified_code, re.DOTALL)

    # Replace the MAC address in the line with the new MAC address
    modified_code = modified_code.replace(mac_address_line.group(), f"uint8_t broadcastAddress[] = {{0x{mac_address}}};")

    with open(output_file_path, "w") as output_file:
        output_file.write(modified_code)

    messagebox.showinfo("Success", "Modified code saved successfully!")

# Function to handle the Build and Flash button click
def build_and_flash():
    os.system("compile.exe")  # Replace "path_to_build_and_flash_script.py" with the actual path to your Python script

# Create the main window
window = tk.Tk()
window.title("ESP-RC TX Flasher")

# Set the window size and make it resizable
window.geometry("400x400")
window.resizable(False, False)

# Configure colors for dark mode
window.configure(bg="#252526")
style = ttk.Style()
style.theme_create("DarkTheme", settings={
    "TLabel": {"configure": {"background": "#252526", "foreground": "white"}},
    "TEntry": {"configure": {"fieldbackground": "#070f22", "foreground": "white"}},
    "TButton": {"configure": {"background": "#0078d7", "foreground": "white"}},
})
style.theme_use("DarkTheme")

# Create labels and entry fields for pins
ppm_pin_label = ttk.Label(window, text="Radio ppm Pin:")
ppm_pin_entry = ttk.Entry(window)
ppm_pin_entry.insert(0, default_ppm_pin)

b1_pin_label = ttk.Label(window, text="Button 1 Pin:")
b1_pin_entry = ttk.Entry(window)
b1_pin_entry.insert(0, default_b1_pin)

b2_pin_label = ttk.Label(window, text="Button 2 Pin:")
b2_pin_entry = ttk.Entry(window)
b2_pin_entry.insert(0, default_b2_pin)

ss_pin_label = ttk.Label(window, text="Lora ss Pin:")
ss_pin_entry = ttk.Entry(window)
ss_pin_entry.insert(0, default_ss)

rst_pin_label = ttk.Label(window, text="Lora rst Pin:")
rst_pin_entry = ttk.Entry(window)
rst_pin_entry.insert(0, default_rst)

dio0_pin_label = ttk.Label(window, text="Lora dio0 Pin:")
dio0_pin_entry = ttk.Entry(window)
dio0_pin_entry.insert(0, default_dio0)

clock_pin_label = ttk.Label(window, text="Display clock Pin:")
clock_pin_entry = ttk.Entry(window)
clock_pin_entry.insert(0, default_clock)

data_pin_label = ttk.Label(window, text="Display data Pin:")
data_pin_entry = ttk.Entry(window)
data_pin_entry.insert(0, default_data)

reset_pin_label = ttk.Label(window, text="Display reset Pin:")
reset_pin_entry = ttk.Entry(window)
reset_pin_entry.insert(0, default_reset)

mac_address_label = ttk.Label(window, text="MAC Address:")
mac_address_entry = ttk.Entry(window)
mac_address_entry.insert(0, default_mac_address)

note_info = ttk.Label(window, text="This may take a while:")
# Create the Save button
save_button = ttk.Button(window, text="Save Pins", command=save_pins)

# Create the Build and Flash button
build_flash_button = ttk.Button(window, text="Build and Flash", command=build_and_flash)

# Grid layout for labels and entry fields
ppm_pin_label.grid(row=0, column=0, padx=10, pady=5, sticky=tk.W + tk.E)
ppm_pin_entry.grid(row=0, column=1, padx=10, pady=5, sticky=tk.W + tk.E)

b1_pin_label.grid(row=1, column=0, padx=10, pady=5, sticky=tk.W + tk.E)
b1_pin_entry.grid(row=1, column=1, padx=10, pady=5, sticky=tk.W + tk.E)

b2_pin_label.grid(row=2, column=0, padx=10, pady=5, sticky=tk.W + tk.E)
b2_pin_entry.grid(row=2, column=1, padx=10, pady=5, sticky=tk.W + tk.E)

ss_pin_label.grid(row=3, column=0, padx=10, pady=5, sticky=tk.W + tk.E)
ss_pin_entry.grid(row=3, column=1, padx=10, pady=5, sticky=tk.W + tk.E)

rst_pin_label.grid(row=4, column=0, padx=10, pady=5, sticky=tk.W + tk.E)
rst_pin_entry.grid(row=4, column=1, padx=10, pady=5, sticky=tk.W + tk.E)

dio0_pin_label.grid(row=5, column=0, padx=10, pady=5, sticky=tk.W + tk.E)
dio0_pin_entry.grid(row=5, column=1, padx=10, pady=5, sticky=tk.W + tk.E)

clock_pin_label.grid(row=6, column=0, padx=10, pady=5, sticky=tk.W + tk.E)
clock_pin_entry.grid(row=6, column=1, padx=10, pady=5, sticky=tk.W + tk.E)

data_pin_label.grid(row=7, column=0, padx=10, pady=5, sticky=tk.W + tk.E)
data_pin_entry.grid(row=7, column=1, padx=10, pady=5, sticky=tk.W + tk.E)

reset_pin_label.grid(row=8, column=0, padx=10, pady=5, sticky=tk.W + tk.E)
reset_pin_entry.grid(row=8, column=1, padx=10, pady=5, sticky=tk.W + tk.E)

mac_address_label.grid(row=9, column=0, padx=10, pady=5, sticky=tk.W + tk.E)
mac_address_entry.grid(row=9, column=1, padx=10, pady=5, sticky=tk.W + tk.E)

note_info.grid(row=10, column=0, columnspan=2, padx=10, pady=5, sticky=tk.W + tk.E)

save_button.grid(row=11, column=0, columnspan=2, padx=10, pady=5, sticky=tk.W + tk.E)
build_flash_button.grid(row=12, column=0, columnspan=2, padx=10, pady=5, sticky=tk.W + tk.E)

# Start the GUI event loop
window.mainloop()
