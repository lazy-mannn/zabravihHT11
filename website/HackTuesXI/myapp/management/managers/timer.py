import time

class TimerManager:
    workingSecCount = 1500
    restSecCount = 300
    snoozeSecCount = 300
    isSnoozed = False
    currentWorkingSecCount = 0
    currentRestSecCount = 0
    currentSnoozeSecCount = 0


    def restCountdown(self):
        currentRestSecCount = self.restSecCount
        while currentRestSecCount:
            time.sleep(1)
            currentRestSecCount -= 1

    def snoozeCountdown(self):
        currentSnoozeSecCount = self.snoozeSecCount
        while currentSnoozeSecCount:
            time.sleep(1)
            currentSnoozeSecCount -= 1
    

    def workingCountdown(self):
        currentWorkingSecCount = self.workingSecCount
        while currentWorkingSecCount:
            if isSnoozed:
                self.snoozeCountdown()
                isSnoozed = False
            time.sleep(1)
            currentWorkingSecCount -= 1

    def snooze(self):
        isSnoozed = True

    def getWorkingSecCount(self):
        return self.currentWorkingSecCount
    
    def getRestSecCount(self):
        return self.currentRestSecCount



    
    