from __future__ import annotations

import streamlit as st


def inject_styles() -> None:
    st.markdown(
        """
        <style>
        .block-container {
            max-width: 1120px;
            padding-top: 2rem;
        }

        div[data-testid="stMetric"] {
            background: #ffffff;
            border: 1px solid #d0d0d0;
            border-radius: 4px;
            padding: 12px;
        }

        .gantt-wrap {
            overflow-x: auto;
            padding-bottom: 8px;
            width: 100%;
        }

        .gantt-rows {
            display: grid;
            gap: 8px;
        }

        .gantt-row {
            align-items: center;
            display: grid;
            gap: 8px;
            grid-template-columns: 46px 1fr;
        }

        .machine-label {
            color: #333333;
            font-size: 13px;
            font-weight: 700;
        }

        .gantt-track {
            background: #fafafa;
            border: 1px solid #dddddd;
            height: 38px;
            position: relative;
        }

        .gantt-block {
            align-items: center;
            background: #2f855a;
            border-right: 1px solid #ffffff;
            color: #ffffff;
            display: flex;
            font-size: 13px;
            font-weight: 700;
            height: 100%;
            justify-content: center;
            min-width: 28px;
            overflow: hidden;
            position: absolute;
            top: 0;
            white-space: nowrap;
        }

        .gantt-axis {
            height: 24px;
            margin-left: 54px;
            position: relative;
        }

        .gantt-axis span {
            color: #555555;
            font-size: 12px;
            position: absolute;
            top: 6px;
            transform: translateX(-50%);
        }
        </style>
        """,
        unsafe_allow_html=True,
    )
