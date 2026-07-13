from __future__ import annotations

import html

import streamlit as st
import streamlit.components.v1 as components


JOB_COLORS = [
    "#2563eb",
    "#16a34a",
    "#dc2626",
    "#9333ea",
    "#ea580c",
    "#0891b2",
    "#4f46e5",
    "#be123c",
    "#0f766e",
    "#a16207",
]


def job_color(job_id: int) -> str:
    return JOB_COLORS[(job_id - 1) % len(JOB_COLORS)]


def render_gantt_chart(simulation: dict | None) -> None:
    segments = (simulation or {}).get("ganttChart") or []
    if not segments:
        st.info("Run the simulation to show the Gantt chart.")
        return

    max_time = max(
        1,
        int((simulation or {}).get("simulationTime") or 0),
        *[int(segment.get("endTime", 0)) for segment in segments],
    )
    machine_ids = sorted({int(segment.get("machineId", 0)) for segment in segments if int(segment.get("machineId", 0)) > 0})
    min_width = max(680, max_time * 64)

    rows: list[str] = []
    for machine_id in machine_ids:
        machine_segments = [segment for segment in segments if int(segment.get("machineId", 0)) == machine_id]
        blocks: list[str] = []
        for segment in machine_segments:
            start = int(segment.get("startTime", 0))
            end = int(segment.get("endTime", start + 1))
            raw_job_id = int(segment.get("jobId", 0))
            job_id = html.escape(str(raw_job_id))
            left = (start / max_time) * 100
            width = max(((end - start) / max_time) * 100, 2)
            color = job_color(raw_job_id)
            blocks.append(
                f"""
                <div class="gantt-block" style="left:{left:.4f}%;width:{width:.4f}%;background:{color};" title="M{machine_id} J{job_id}: {start} to {end}">
                    <strong>J{job_id}</strong>
                    <span>{start}-{end}</span>
                </div>
                """
            )
        rows.append(
            f"""
            <div class="gantt-row">
                <div class="machine-label">M{machine_id}</div>
                <div class="gantt-track">{''.join(blocks)}</div>
            </div>
            """
        )

    axis_marks = []
    for time in range(max_time + 1):
        left = (time / max_time) * 100
        axis_marks.append(f'<span style="left:{left:.4f}%">{time}</span>')

    chart_height = 72 + (len(machine_ids) * 48)
    components.html(
        f"""
        <!doctype html>
        <html>
        <head>
          <style>
            body {{
              font-family: Arial, Helvetica, sans-serif;
              margin: 0;
              overflow-x: auto;
              overflow-y: hidden;
            }}

            .gantt-wrap {{
              overflow-x: auto;
              padding: 4px 0 8px;
              width: 100%;
            }}

            .gantt-rows {{
              display: grid;
              gap: 12px;
            }}

            .gantt-row {{
              align-items: center;
              display: grid;
              gap: 8px;
              grid-template-columns: 46px 1fr;
            }}

            .machine-label {{
              color: #333333;
              font-size: 13px;
              font-weight: 700;
            }}

            .gantt-track {{
              background:
                repeating-linear-gradient(
                  to right,
                  #f8fafc 0,
                  #f8fafc 62px,
                  #e5e7eb 63px,
                  #f8fafc 64px
                );
              border: 1px solid #dddddd;
              border-radius: 6px;
              height: 44px;
              position: relative;
            }}

            .gantt-block {{
              align-items: center;
              border: 3px solid #ffffff;
              border-radius: 8px;
              box-shadow: 0 1px 3px rgba(15, 23, 42, 0.25);
              box-sizing: border-box;
              color: #ffffff;
              display: flex;
              flex-direction: column;
              font-size: 13px;
              font-weight: 700;
              height: 38px;
              justify-content: center;
              min-width: 28px;
              overflow: hidden;
              position: absolute;
              top: 3px;
              white-space: nowrap;
            }}

            .gantt-block span {{
              display: block;
              font-size: 10px;
              font-weight: 600;
              line-height: 1;
              opacity: 0.9;
            }}

            .gantt-axis {{
              height: 24px;
              margin-left: 54px;
              position: relative;
            }}

            .gantt-axis span {{
              color: #555555;
              font-size: 12px;
              position: absolute;
              top: 6px;
              transform: translateX(-50%);
            }}
          </style>
        </head>
        <body>
          <div class="gantt-wrap">
            <div class="gantt-rows" style="min-width:{min_width}px;">
              {''.join(rows)}
            </div>
            <div class="gantt-axis" style="min-width:{min_width}px;">
              {''.join(axis_marks)}
            </div>
          </div>
        </body>
        </html>
        """,
        height=chart_height,
        scrolling=True,
    )
