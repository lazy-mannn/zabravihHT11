import time
from myapp.management.managers.timer_status_manager import Status

class TimerManager:
    def __init__(self, status_manager):
        self.status_manager = status_manager
        self.workingSecCount = 60
        self.restSecCount = 30
        self.snoozeSecCount = 30
        self.currentWorkingSecCount = self.workingSecCount
        self.currentRestSecCount = self.restSecCount
        self.currentSnoozeSecCount = self.snoozeSecCount

    def restCountdown(self):
        self.currentRestSecCount = self.restSecCount
        while self.currentRestSecCount > 0:
            time.sleep(1)
            self.currentRestSecCount -= 1
        self.status_manager.set_status(Status.INITIAL.name)

    def snoozeCountdown(self):
        self.currentSnoozeSecCount = self.snoozeSecCount
        while self.currentSnoozeSecCount > 0:
            time.sleep(1)
            self.currentSnoozeSecCount -= 1
        self.status_manager.set_status(Status.WORK_IN_PROGRESS.name)
        self.tick()

    def workingCountdown(self):
        if self.currentWorkingSecCount <= 0:
            self.currentWorkingSecCount = self.workingSecCount
        while self.currentWorkingSecCount > 0:
            if self.status_manager.get_status() == Status.PAUSED.name:
                self.tick()
                return
            time.sleep(1)
            self.currentWorkingSecCount -= 1
        if self.status_manager.get_status() == Status.WORK_IN_PROGRESS.name:
            self.status_manager.set_status(Status.REST.name)

        self.tick()

    def getWorkingSecCount(self):
        return self.currentWorkingSecCount
    
    def getRestSecCount(self):
        return self.currentRestSecCount
    
    def getSnoozeSecCount(self):
        return self.currentSnoozeSecCount

    def tick(self):
        if self.status_manager.get_status() == Status.WORK_IN_PROGRESS.name:
            self.workingCountdown()
        elif self.status_manager.get_status() == Status.REST.name:
            self.restCountdown()
        elif self.status_manager.get_status() == Status.PAUSED.name:
            self.snoozeCountdown()
