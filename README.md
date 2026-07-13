# Factory Job Scheduler

A multi-machine factory scheduling simulator built with a C++ backend, a Node.js JSON bridge, and two independent frontends:

- React + TypeScript + Vite frontend in `web/`
- Streamlit frontend in `factory_scheduler_streamlit/`

The project supports:

- FCFS
- Shortest Job First (Non-Preemptive)
- Priority Scheduling (Non-Preemptive)
- Round Robin
- Multiple machines
- Arrival time
- Burst/processing time
- Priority
- Time quantum
- Waiting time
- Turnaround time
- Throughput
- Machine utilization
- Gantt chart visualization

## Tech Stack

Backend:

- C++
- Object-oriented programming
- JSON file input/output

Bridge/API:

- Node.js
- Built-in `http`, `fs`, and `child_process` modules

Frontend options:

- React
- TypeScript
- Vite
- CSS
- Streamlit
- Python

## Project Structure

```text
FactoryScheduler/
  main.cpp
  Job.cpp / Job.h
  Machine.cpp / Machine.h
  Factory.cpp / Factory.h
  Scheduler.cpp / Scheduler.h
  FCFSScheduler.cpp / FCFSScheduler.h
  SJFScheduler.cpp / SJFScheduler.h
  PriorityScheduler.cpp / PriorityScheduler.h
  RoundRobinScheduler.cpp / RoundRobinScheduler.h
  Simulation.cpp / Simulation.h
  Statistics.cpp / Statistics.h
  input.json
  simulation.json
  web/
    server.js
    package.json
    src/
    dist/
  factory_scheduler_streamlit/
    app.py
    requirements.txt
    README.md
    components/
    utils/
```

## How It Works

1. The frontend sends scheduler input to the Node.js server.
2. The Node.js server writes the input to `input.json`.
3. The Node.js server runs the C++ executable `FactoryScheduler.exe`.
4. The C++ backend reads `input.json`, runs the simulation, and writes `simulation.json`.
5. The Node.js server reads `simulation.json`.
6. The frontend displays metrics and the Gantt chart.

## Input JSON Format

```json
{
  "machines": 2,
  "scheduler": "RR",
  "timeQuantum": 2,
  "jobs": [
    {
      "id": 1,
      "processingTime": 5,
      "arrivalTime": 0,
      "priority": 3
    }
  ]
}
```

Supported scheduler values:

```text
FCFS
SJF
PRIORITY
RR
```

## Output JSON Includes

- Completed jobs
- Simulation time
- Average waiting time
- Average turnaround time
- Throughput
- Job metrics
- Gantt chart data
- Machine utilization
- Event log

## Run the React Frontend

Open PowerShell:

```powershell
cd D:\FactoryScheduler\web
npm.cmd install
npm.cmd run build
node server.js
```

Then open:

```text
http://localhost:5173
```

## Run the Streamlit Frontend

First start the existing backend/API server:

```powershell
cd D:\FactoryScheduler\web
node server.js
```

In another PowerShell window:

```powershell
cd D:\FactoryScheduler\factory_scheduler_streamlit
python -m venv .venv
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
.\.venv\Scripts\Activate.ps1
pip install -r requirements.txt
streamlit run app.py
```

Then open the Streamlit URL shown in the terminal, usually:

```text
http://localhost:8501
```

## Rebuild the C++ Backend

If you modify any C++ file, rebuild the executable:

```powershell
cd D:\FactoryScheduler
g++ -std=c++17 -Wall -Wextra -o FactoryScheduler.exe main.cpp Factory.cpp FCFSScheduler.cpp Job.cpp Machine.cpp PriorityScheduler.cpp RoundRobinScheduler.cpp Scheduler.cpp Simulation.cpp SJFScheduler.cpp Statistics.cpp
```

## GitHub Upload Recommendation

Upload the full project folder so the backend, API bridge, React frontend, and Streamlit frontend are all available together.

Do upload:

```text
*.cpp
*.h
web/
factory_scheduler_streamlit/
input.json
simulation.json
README.md
.gitignore
```

Do not upload:

```text
node_modules/
.venv/
__pycache__/
web/dist/
*.pyc
.env
```

Optional:

- You may upload `FactoryScheduler.exe` if you want users on Windows to run without compiling.
- For a cleaner source-code repository, do not upload `.exe` files and ask users to compile.

## Suggested GitHub Description

```text
Multi-machine factory job scheduling simulator using C++ OOP backend, Node.js JSON bridge, React frontend, and Streamlit frontend.
```

## Resume Summary

Built a multi-machine Factory Job Scheduler using C++ OOP and React/Streamlit frontends. Implemented FCFS, SJF, Priority Scheduling, and Round Robin algorithms with JSON-based communication, Gantt chart visualization, waiting time, turnaround time, throughput, and machine utilization metrics.

