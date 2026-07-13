from __future__ import annotations

import pandas as pd
import streamlit as st
from dotenv import load_dotenv

from components.gantt import render_gantt_chart
from components.styles import inject_styles
from utils.api import ApiError, fetch_state, run_simulation
from utils.defaults import ALGORITHMS, DEFAULT_TIME_QUANTUM, default_jobs


load_dotenv()


def normalize_int(value: object, fallback: int, minimum: int | None = None) -> int:
    try:
        parsed = int(value)
    except (TypeError, ValueError):
        parsed = fallback
    if minimum is not None:
        return max(minimum, parsed)
    return parsed


def average_machine_utilization(simulation: dict | None) -> float:
    if not simulation:
        return 0.0
    utilization = simulation.get("machineUtilization") or []
    if not utilization:
        return 0.0
    return sum(float(machine.get("utilizationPercent", 0)) for machine in utilization) / len(utilization)


def load_initial_state() -> None:
    if st.session_state.get("initialized"):
        return

    st.session_state.initialized = True
    st.session_state.algorithm = "FCFS"
    st.session_state.machines = 1
    st.session_state.time_quantum = DEFAULT_TIME_QUANTUM
    st.session_state.jobs = default_jobs(4)
    st.session_state.simulation = None
    st.session_state.load_error = ""

    try:
        state = fetch_state()
        api_input = state.get("input")
        if api_input:
            st.session_state.algorithm = api_input.get("scheduler") or "FCFS"
            st.session_state.machines = normalize_int(api_input.get("machines"), 1, 1)
            st.session_state.time_quantum = normalize_int(api_input.get("timeQuantum"), DEFAULT_TIME_QUANTUM, 1)
            jobs = api_input.get("jobs") or []
            st.session_state.jobs = jobs if jobs else default_jobs(4)
        st.session_state.simulation = state.get("simulation")
    except ApiError as exc:
        st.session_state.load_error = str(exc)


def sync_job_count(count: int) -> None:
    current = list(st.session_state.jobs)
    if count == len(current):
        return
    if count < len(current):
        st.session_state.jobs = current[:count]
        return

    new_jobs = []
    for index in range(count - len(current)):
        job_id = len(current) + index + 1
        new_jobs.append(
            {
                "id": job_id,
                "arrivalTime": job_id - 1,
                "processingTime": 2,
                "priority": job_id,
            }
        )
    st.session_state.jobs = current + new_jobs


def jobs_to_dataframe(show_priority: bool) -> pd.DataFrame:
    rows = []
    for job in st.session_state.jobs:
        row = {
            "Job": f"J{job['id']}",
            "Arrival Time": int(job.get("arrivalTime", 0)),
            "Burst Time": int(job.get("processingTime", 1)),
        }
        if show_priority:
            row["Priority"] = int(job.get("priority", 1))
        rows.append(row)
    return pd.DataFrame(rows)


def dataframe_to_jobs(df: pd.DataFrame) -> list[dict]:
    next_jobs: list[dict] = []
    existing_jobs = list(st.session_state.jobs)
    for index, row in df.reset_index(drop=True).iterrows():
        existing = existing_jobs[index] if index < len(existing_jobs) else {}
        next_jobs.append(
            {
                "id": index + 1,
                "arrivalTime": normalize_int(row.get("Arrival Time"), 0, 0),
                "processingTime": normalize_int(row.get("Burst Time"), 1, 1),
                "priority": normalize_int(row.get("Priority"), existing.get("priority", index + 1)),
            }
        )
    return next_jobs


def build_payload() -> dict:
    return {
        "machines": int(st.session_state.machines),
        "scheduler": st.session_state.algorithm,
        "timeQuantum": int(st.session_state.time_quantum),
        "jobs": st.session_state.jobs,
    }


st.set_page_config(
    page_title="Factory Scheduling Simulator",
    page_icon="🏭",
    layout="wide",
)

inject_styles()
load_initial_state()

st.title("Factory Scheduling Simulator")
st.caption("Configure jobs and machines, run the scheduler, and compare the core factory scheduling metrics.")

if st.session_state.load_error:
    st.warning(
        "Could not load the current JSON state from the backend. "
        "Start the existing Node server first, then refresh this Streamlit app.\n\n"
        f"Details: {st.session_state.load_error}"
    )

control_cols = st.columns([2, 1, 1, 1])
with control_cols[0]:
    selected_label = st.selectbox(
        "Scheduling Algorithm",
        options=[algorithm["label"] for algorithm in ALGORITHMS],
        index=next(
            (
                index
                for index, algorithm in enumerate(ALGORITHMS)
                if algorithm["value"] == st.session_state.algorithm
            ),
            0,
        ),
    )
    st.session_state.algorithm = next(
        algorithm["value"] for algorithm in ALGORITHMS if algorithm["label"] == selected_label
    )

with control_cols[1]:
    job_count = st.number_input(
        "Number of Jobs",
        min_value=1,
        step=1,
        value=len(st.session_state.jobs),
    )
    sync_job_count(int(job_count))

with control_cols[2]:
    st.session_state.machines = st.number_input(
        "Number of Machines",
        min_value=1,
        step=1,
        value=int(st.session_state.machines),
    )

with control_cols[3]:
    if st.session_state.algorithm == "RR":
        st.session_state.time_quantum = st.number_input(
            "Time Quantum",
            min_value=1,
            step=1,
            value=int(st.session_state.time_quantum),
        )
    else:
        st.empty()

show_priority = st.session_state.algorithm == "PRIORITY"

st.subheader("Input Parameters")
edited_jobs = st.data_editor(
    jobs_to_dataframe(show_priority),
    hide_index=True,
    use_container_width=True,
    disabled=["Job"],
    column_config={
        "Job": st.column_config.TextColumn("Job"),
        "Arrival Time": st.column_config.NumberColumn("Arrival Time", min_value=0, step=1),
        "Burst Time": st.column_config.NumberColumn("Burst Time", min_value=1, step=1),
        "Priority": st.column_config.NumberColumn("Priority", step=1),
    },
)
st.session_state.jobs = dataframe_to_jobs(edited_jobs)

run_col, status_col = st.columns([1, 4])
with run_col:
    run_clicked = st.button("Run Simulation", type="primary", use_container_width=True)

if run_clicked:
    try:
        with st.spinner("Running scheduler..."):
            response = run_simulation(build_payload())
        st.session_state.simulation = response.get("state", {}).get("simulation")
        status_col.success("Simulation completed.")
    except ApiError as exc:
        status_col.error(str(exc))

simulation = st.session_state.simulation

metric_values = [
    ("Simulation Time", int(simulation.get("simulationTime", 0)) if simulation else 0),
    ("Throughput", f"{float(simulation.get('throughput', 0)):.2f}" if simulation else "0.00"),
    ("Avg Waiting Time", f"{float(simulation.get('averageWaitingTime', 0)):.2f}" if simulation else "0.00"),
    ("Avg Turnaround Time", f"{float(simulation.get('averageTurnaroundTime', 0)):.2f}" if simulation else "0.00"),
    ("Machine Utilization", f"{average_machine_utilization(simulation):.2f}%"),
]

metric_cols = st.columns(len(metric_values))
for column, (label, value) in zip(metric_cols, metric_values):
    column.metric(label, value)

st.subheader("Gantt Chart")
render_gantt_chart(simulation)

if simulation and simulation.get("jobMetrics"):
    st.subheader("Job Results")
    result_rows = [
        {
            "Job": f"J{metric.get('jobId')}",
            "Completion": metric.get("completionTime"),
            "Turnaround": metric.get("turnaroundTime"),
            "Waiting": metric.get("waitingTime"),
        }
        for metric in simulation.get("jobMetrics", [])
    ]
    st.dataframe(pd.DataFrame(result_rows), hide_index=True, use_container_width=True)
