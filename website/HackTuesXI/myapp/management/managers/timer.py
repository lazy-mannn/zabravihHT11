import time

class TimerManager:
    def __init__(self):
        self.workingSecCount = 1500
        self.restSecCount = 300
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
        self.currentSnoozeSecCount = self.snoozeSecCount
        while self.currentSnoozeSecCount > 0:
            time.sleep(1)
            self.currentSnoozeSecCount -= 1

    def workingCountdown(self):
        self.currentWorkingSecCount = self.workingSecCount
        while self.currentWorkingSecCount > 0:
            if self.isSnoozed:
                self.snoozeCountdown()
                self.isSnoozed = False
            time.sleep(1)
            self.currentWorkingSecCount -= 1

    def snooze(self):
        self.isSnoozed = True

    def getWorkingSecCount(self):
        return self.currentWorkingSecCount
    
    def getRestSecCount(self):
        return self.currentRestSecCount
