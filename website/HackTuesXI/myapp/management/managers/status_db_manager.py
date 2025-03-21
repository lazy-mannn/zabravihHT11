from myapp.models import TimerStatus

class StatusDBManager:
    def __init__(self):
        if not TimerStatus.objects.exists():
            TimerStatus.objects.create(current_status="INITIAL")
        else:
            self.reset()

    def reset(self):
        status = TimerStatus.objects.first()
        status.current_status = "INITIAL"
        status.save()

    def set_status(self, new_status: str):
        status = TimerStatus.objects.first()
        status.current_status = new_status
        status.save()

    def get_status(self) -> str:
        return TimerStatus.objects.first().current_status
