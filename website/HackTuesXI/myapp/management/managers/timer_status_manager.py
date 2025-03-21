import json
from enum import Enum

class Status(Enum):
    WORK_IN_PROGRESS = "Work in progress"
    REST = "Break"
    PAUSED = "Paused"
    INITIAL = "Initial"

    def to_json(self):
        return self.value

    @staticmethod
    def from_json(data):
        for status in Status:
            if status.value == data:
                return status
        raise ValueError(f"Unknown status: {data}")

class TimerStatusManager:
    def __init__(self):
        self.status = Status.INITIAL

    def set_status(self, status: Status):
        self.status = status

    def get_status(self) -> Status:
        return self.status

    def to_json(self):
        return json.dumps({"status": self.status.to_json()})

    @staticmethod
    def from_json(data):
        obj = json.loads(data)
        status = Status.from_json(obj["status"])
        manager = TimerStatusManager()
        manager.set_status(status)
        return manager
