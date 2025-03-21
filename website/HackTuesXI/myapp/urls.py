from django.contrib import admin
from django.urls import path
from . import views

urlpatterns = [
    path('', views.timer),
    path('get-status/', views.get_status, name='get_status'),
    path('start-work/', views.start_work, name='start_work'),
    path('snooze/', views.snooze, name='snooze'),
]
