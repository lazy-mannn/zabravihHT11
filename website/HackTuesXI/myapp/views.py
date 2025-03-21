from django.shortcuts import render
from django.http import JsonResponse
from myapp.management.managers.status_db_manager import StatusDBManager
from myapp.management.managers.timer import TimerManager

status_manager = StatusDBManager()
timer_manager = TimerManager(status_manager)

def index(request):
    return render(request, "myapp/index.html")

def start_work(request):
    status_manager.set_status("WORK_IN_PROGRESS")
    timer_manager.tick()
    return JsonResponse({"message": "started"})

def get_status(request):
    return JsonResponse({
        "status": status_manager.get_status(),
        "work_time": timer_manager.getWorkingSecCount(),
        "break_time": timer_manager.getRestSecCount(),
        "snooze_time": timer_manager.getSnoozeSecCount()
    })
