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
