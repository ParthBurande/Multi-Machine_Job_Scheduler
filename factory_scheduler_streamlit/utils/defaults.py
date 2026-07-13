from __future__ import annotations


DEFAULT_TIME_QUANTUM = 2

ALGORITHMS = [
    {"label": "FCFS", "value": "FCFS"},
    {"label": "SJF (Non-Preemptive)", "value": "SJF"},
    {"label": "Priority (Non-Preemptive)", "value": "PRIORITY"},
    {"label": "Round Robin", "value": "RR"},
]


def default_jobs(count: int) -> list[dict]:
    return [
        {
            "id": index + 1,
            "arrivalTime": index,
            "processingTime": 2,
            "priority": index + 1,
        }
        for index in range(count)
    ]
