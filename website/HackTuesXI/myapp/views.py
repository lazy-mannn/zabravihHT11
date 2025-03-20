from django.shortcuts import render

from django.shortcuts import render
def timer(request):
    return render(request, "myapp/timer.html")