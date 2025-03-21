import time
from myapp.management.managers.timer_status_manager import Status

class TimerManager:
    def __init__(self, status_manager):
        self.status_manager = status_manager
        self.workingSecCount = 10
        self.restSecCount = 5
        self.snoozeSecCount = 300
        self.isSnoozed = False
        self.currentWorkingSecCount = self.workingSecCount
        self.currentRestSecCount = self.restSecCount
        self.currentSnoozeSecCount = self.snoozeSecCount

    def restCountdown(self):
        while self.currentRestSecCount > 0:
            time.sleep(1)
            self.currentRestSecCount -= 1

    def snoozeCountdown(self):
        self.isSnoozed = True
        self.currentSnoozeSecCount = self.snoozeSecCount
        while self.currentSnoozeSecCount > 0:
            time.sleep(1)
            self.currentSnoozeSecCount -= 1
        self.isSnoozed = False

    def workingCountdown(self):
        self.currentWorkingSecCount = self.workingSecCount
        while self.currentWorkingSecCount > 0:
            if self.isSnoozed:
                continue
            time.sleep(1)
            self.currentWorkingSecCount -= 1
        self.status_manager.set_status(Status.REST)
        self.tick()  # Call tick again to start the rest timer

    def snooze(self):
        self.isSnoozed = True

    def getWorkingSecCount(self):
        return self.currentWorkingSecCount
    
    def getRestSecCount(self):
        return self.currentRestSecCount

    def tick(self):
        if self.status_manager.get_status() == Status.WORK_IN_PROGRESS:
            self.workingCountdown()
        elif self.status_manager.get_status() == Status.REST:
            self.restCountdown()
        elif self.status_manager.get_status() == Status.PAUSED:
            self.snoozeCountdown()
