from time import sleep
from yamcs.client import YamcsClient
from yamcs.tmtc.model import VerificationConfig

def arm_debug():
    command = processor.issue_command("/cfs/cpd/apps/amc/ArmDebug")
    print("DebugArmed", command)

def engage_debug():
    command = processor.issue_command("/cfs/cpd/apps/amc/EngageDebug")
    print("EngageDebug", command)

def debug_cmd(index, cmd):
    command = processor.issue_command(
        "/cfs/cpd/apps/amc/DebugCmd", args={"Index": index, "Cmd": cmd}
    )
    print("Issued", command)

if __name__ == "__main__":
    armed = 1100
    disarmed = 900

    client = YamcsClient("localhost:8090")
    processor = client.get_processor("fsw", "realtime")

    arm_debug()
    sleep(1)
    engage_debug()
    sleep(1)

    for i in range(8):
        debug_cmd(i, armed)
        sleep(2)
        debug_cmd(i, disarmed)

