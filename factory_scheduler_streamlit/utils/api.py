from __future__ import annotations

import os
from typing import Any

import requests


class ApiError(Exception):
    """Raised when the existing Factory Scheduler API cannot complete a request."""


def api_base_url() -> str:
    return os.getenv("FACTORY_SCHEDULER_API_BASE", "http://localhost:5173").rstrip("/")


def _request(method: str, path: str, **kwargs: Any) -> dict:
    url = f"{api_base_url()}{path}"
    try:
        response = requests.request(method, url, timeout=20, **kwargs)
    except requests.RequestException as exc:
        raise ApiError(
            f"Could not reach Factory Scheduler backend at {api_base_url()}. "
            "Start it with: cd D:\\FactoryScheduler\\web && node server.js"
        ) from exc

    try:
        payload = response.json()
    except ValueError as exc:
        raise ApiError(f"Backend returned a non-JSON response from {url}.") from exc

    if not response.ok:
        message = payload.get("error") or payload.get("details") or f"Request failed with HTTP {response.status_code}."
        raise ApiError(message)

    return payload


def fetch_state() -> dict:
    return _request("GET", "/api/state")


def run_simulation(payload: dict) -> dict:
    return _request("POST", "/api/simulate", json=payload)
