import dash
from dash import html, dcc
import plotly.graph_objects as go
from flask import Flask, request, Response
from threading import Timer
import datetime
import webbrowser

import logging
from flask.logging import default_handler



class Plotter:
    def __init__(self):
        self.data = {}
        self.port = 8050
        self.server = Flask(__name__)

        self.app = dash.Dash(__name__, server=self.server)

        self.app.layout = html.Div([
            dcc.Graph(id='live-update-graph', figure=go.Figure()),
            dcc.Interval(
                id='interval-component',
                interval=2000,  # Update every 2 seconds
                n_intervals=0
            )
        ])

        self.fig = self.app.layout['live-update-graph'].figure
        self.app.server.route('/add-data', methods=['POST'])(self.add_data)

        self.app.callback(
            dash.dependencies.Output('live-update-graph', 'figure'),
            [dash.dependencies.Input('interval-component', 'n_intervals')]
        )(self.update_plot)
        # Get the Flask server logger
        logger = logging.getLogger('werkzeug')
        logger.setLevel(logging.ERROR)  # Set the logging level to ERROR

        # Remove the default Flask logger handler
        logger.removeHandler(default_handler)

    def plot(self):
        self.fig.show()
    def open_browser(self):
	    webbrowser.open_new("http://localhost:{}".format(self.port))

    def add_data(self):
        if request.method == 'POST':
            data = request.get_json()
            time = data.pop("time")
            for key, value in data.items():
                if key in self.data:
                    self.data[key]["data"].append(value)
                    self.data[key]["time"].append(time)
                else:
                    self.data[key] = {"data": [value], "time": [time]}
                    trace = go.Scatter(
                        x=self.data[key]['time'],
                        y=self.data[key]['data'],
                        name=key,
                        mode='lines+markers'
                    )
                    self.fig.add_trace(trace)

            return "Data received"
        else:
            return "Invalid request method"

    def update_plot(self, n_intervals):
        for key in self.data:
            trace_idx = next((idx for idx, trace in enumerate(self.fig.data) if trace.name == key), None)
            self.fig.data[trace_idx].x = self.data[key]["time"]
            self.fig.data[trace_idx].y = self.data[key]["data"]

        return self.fig

    def run(self):
        Timer(1, self.open_browser).start()
        self.app.run_server(debug=False, dev_tools_ui=False)

if __name__ == '__main__':
    plotter = Plotter()
    plotter.run()
    plotter.add_data({"time": 1, "current.in_sun": 1, "temp.in_sun": 0})
    plotter.add_data({"time": 2, "current.in_sun": 2, "temp.in_sun": 2})
    plotter.add_data({"time": 3, "current.in_sun": 3, "temp.in_sun": 4})
