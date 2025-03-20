from enum import Enum

class Status(Enum):
    WORK_IN_PROGRESS = 1
    REST = 2
    PAUSED = 3

class TimerStatusManager:
    status: Status
    def __init__(self):
        self.status = Status.REST

    def set_status(self, status: Status):
        self.status = status

    def get_status(self) -> Status:
        return self.status
    
    