from django.db import models

class TimerStatus(models.Model):
    STATUS_CHOICES = [
        ("INITIAL", "Initial"),
        ("WORK_IN_PROGRESS", "Work in progress"),
        ("PAUSED", "Paused"),
        ("REST", "Break"),
    ]

    current_status = models.CharField(
        max_length=32,
        choices=STATUS_CHOICES,
        default="INITIAL"
    )
    updated_at = models.DateTimeField(auto_now=True)

    def __str__(self):
        return f"{self.current_status} (updated at {self.updated_at})"
