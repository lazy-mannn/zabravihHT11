from django.shortcuts import render
from django.http import JsonResponse
from myapp.management.managers.timer_status_manager import TimerStatusManager, Status
from myapp.management.managers.timer import TimerManager

status_manager = TimerStatusManager()
timer_manager = TimerManager(status_manager)

def index(request):
    return render(request, "myapp/index.html")

def start_work(request):
    status_manager.set_status(Status.WORK_IN_PROGRESS)
    timer_manager.tick()
    return JsonResponse({"message": "started"})

def get_status(request):
    return JsonResponse({
        "status": status_manager.get_status().to_json(),
        "work_time": timer_manager.getWorkingSecCount(),
        "break_time": timer_manager.getRestSecCount()
    })
