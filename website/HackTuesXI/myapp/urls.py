from django.urls import path
from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('start-work/', views.start_work, name='start_work'),
    path('get-status/', views.get_status, name='get_status'),
]
