import json
from pathlib import Path
import time

import bluetooth as bt

DOT = Path(__file__).parent
DIR_SENT = "0x00"
START_FROM = 0
STOP_AFTER = None

with open(DOT / "packets.json") as f:
    packets = json.load(f)

sock = bt.BluetoothSocket(bt.RFCOMM)
sock.connect(("2C:BE:EB:DE:F0:55", 15))  # replace with your address

for packet in packets:
    if packet["_source"]["layers"]["hci_h4"]["hci_h4.direction"] != DIR_SENT:
        continue
    num = int(packet["_source"]["layers"]["frame"]["frame.number"])
    if num < START_FROM:
        continue
    if STOP_AFTER is not None and num > STOP_AFTER:
        continue
    print(f"Sending packet {num}")
    try:
        data = packet["_source"]["layers"]["data"]["data.data"]
    except KeyError:
        print("No data in packet... skipping.\n")
        continue
    print(f"Data: {data}")
    if "pkt_comment" in packet["_source"]["layers"]:
        print(f"Comment: {packet["_source"]["layers"]["pkt_comment"]["frame.comment"]}")
    data_bytes = bytes(int(x, 16) for x in data.split(":"))
    sock.send(data_bytes)
    # sock.recv_all()
    print()
    time.sleep(2)
