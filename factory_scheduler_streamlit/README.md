# Factory Scheduler Streamlit UI

This folder contains a standalone Streamlit implementation of the Factory Scheduler UI.

It does **not** replace or modify the existing React application. The React app remains in `D:\FactoryScheduler\web` and continues to work as before.

## What This App Does

The Streamlit app recreates the current Factory Scheduler workflow:

- Select scheduling algorithm
- Configure number of jobs
- Configure number of machines
- Edit arrival time and burst time
- Edit priority when Priority Scheduling is selected
- Edit time quantum when Round Robin is selected
- Run simulation
- Display simulation time, throughput, average waiting time, average turnaround time, and machine utilization
- Display a multi-machine Gantt chart
- Display job result table

## Backend/API Reuse

This app reuses the existing Node.js bridge and C++ backend.

It calls the same endpoints used by the React frontend:

- `GET /api/state`
- `POST /api/simulate`

The backend behavior, JSON format, scheduling logic, and validation rules are unchanged.

## Install Dependencies

Open PowerShell in this folder:

```powershell
cd D:\FactoryScheduler\factory_scheduler_streamlit
python -m venv .venv
.\.venv\Scripts\Activate.ps1
pip install -r requirements.txt
```

If PowerShell blocks activation, run:

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
.\.venv\Scripts\Activate.ps1
```

## Start the Existing Backend

The Streamlit app expects the existing Node backend to be running.

In a separate PowerShell window:

```powershell
cd D:\FactoryScheduler\web
node server.js
```

You should see:

```text
Factory Scheduler UI running at http://localhost:5173
```

## Run the Streamlit App

In the Streamlit folder:

```powershell
cd D:\FactoryScheduler\factory_scheduler_streamlit
.\.venv\Scripts\Activate.ps1
streamlit run app.py
```

Streamlit will open a browser window, usually at:

```text
http://localhost:8501
```

## Configuration

By default, the Streamlit app connects to:

```text
http://localhost:5173
```

To override this, create a `.env` file based on `.env.example`:

```powershell
copy .env.example .env
```

Then edit:

```text
FACTORY_SCHEDULER_API_BASE=http://localhost:5173
```

## Notes

- Keep the Node backend running while using Streamlit.
- Do not run Streamlit as a replacement for the backend; it is only another UI.
- The React UI and Streamlit UI are independent frontends that connect to the same backend API.
