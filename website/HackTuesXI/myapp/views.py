from django.http import JsonResponse
from django.shortcuts import render
from myapp.management.managers.timer_status_manager import TimerStatusManager, Status
from myapp.management.managers.timer import TimerManager

status_manager = TimerStatusManager()
timer_manager = TimerManager()

def timer(request):
    return render(request, "myapp/timer.html")

def get_status(request):
    return JsonResponse({
        "status": status_manager.get_status().name,
        "working_time_left": timer_manager.getWorkingSecCount(),
        "rest_time_left": timer_manager.getRestSecCount()
    })

def start_work(request):
    status_manager.set_status(Status.WORK_IN_PROGRESS)
    return JsonResponse({"message": "Work started"})

def snooze(request):
    status_manager.set_status(Status.PAUSED)
    timer_manager.snoozeCountdown()
    status_manager.set_status(Status.WORK_IN_PROGRESS)
    return JsonResponse({"message": "Snoozed and resumed"})
