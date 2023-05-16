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
        

        # Remove logging so it doesn't interfere with the debug console.
        logger = logging.getLogger('werkzeug')
        logger.setLevel(logging.ERROR)  # Set the logging level to ERROR

        # Remove the default Flask logger handler
        logger.removeHandler(default_handler)

    def plot(self):
        self.fig.show()
    
    # Open the plots
    def open_browser(self):
	    webbrowser.open_new("http://localhost:{}".format(self.port))


    # Add the next set of data to the plots
    def add_data(self):
        if request.method == 'POST':
            data = request.get_json()
            time = data.pop("time")
            for key, value in data.items():
                # Append to already existing trace
                if key in self.data:
                    self.data[key]["data"].append(value)
                    self.data[key]["time"].append(time)
                else:
                    # Create new trace for new field being logged
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

    # update the plots with the new data
    def update_plot(self, n_intervals):
        for key in self.data:
            # Iterate over each trace to find the trace and then add the new data
            trace_idx = next((idx for idx, trace in enumerate(self.fig.data) if trace.name == key), None)
            self.fig.data[trace_idx].x = self.data[key]["time"]
            self.fig.data[trace_idx].y = self.data[key]["data"]

        return self.fig

    # Start the plotter
    def run(self):
        Timer(1, self.open_browser).start()
        self.app.run_server(debug=False, dev_tools_ui=False)

# Test for basic plot data
if __name__ == '__main__':
    plotter = Plotter()
    plotter.run()
    plotter.add_data({"time": 1, "current.in_sun": 1, "temp.in_sun": 0})
    plotter.add_data({"time": 2, "current.in_sun": 2, "temp.in_sun": 2})
    plotter.add_data({"time": 3, "current.in_sun": 3, "temp.in_sun": 4})
