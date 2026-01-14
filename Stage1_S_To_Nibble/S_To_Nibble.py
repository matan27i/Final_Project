import serial
import time


def send_value_as_nibbles():
    port = 'COM5'
    baudrate = 9600

    try:
        # פתיחת הפורט פעם אחת בלבד
        with serial.Serial(port, baudrate, timeout=1) as ser:
            print(f"Connected to {port}. Initializing...")
            time.sleep(2)

            user_input = input("Enter a value (Decimal 0-4095 or Hex 0-F): ").strip()

            try:
                # 1. המרה לערך הקסדצימלי (ללא ה-0x) באותיות גדולות
                if user_input.startswith(('0x', '0X')):
                    hex_str = user_input[2:].upper()
                elif user_input.isdigit():
                    hex_str = f"{int(user_input):X}"
                else:
                    hex_str = user_input.upper()

                # 2. בדיקת מגבלת ה-Buffer של ה-MCU (MAX_PACKETS = 3)
                if len(hex_str) > 3:
                    print("Warning: Input exceeds MAX_PACKETS (3). Only the first 3 nibbles will be processed.")
                    hex_str = hex_str[:3]

                # 3. שליחת כל ניבל כתו ASCII
                for char in hex_str:
                    if char in "0123456789ABCDEF":
                        print(f"Sending Nibble: {char}")
                        ser.write(char.encode('ascii'))
                        time.sleep(0.1)  # השהייה לעיבוד ה-ISR ב-MCU

                # 4. שליחת תו סוף שורה להפעלת ה-buffer_flag
                ser.write(b'\n')
                print("Transmission complete (Newline sent).")

            except ValueError:
                print("Invalid input. Please enter a number or a hex string.")

    except serial.SerialException as e:
        print(f"Serial Error: {e}")


if __name__ == "__main__":
    send_value_as_nibbles()