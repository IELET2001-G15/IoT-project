var ctx = document.getElementById('myChart').getContext('2d'); //Defines the basic graphic element of the graph

var myLineChart = new Chart(ctx, { //Defines the graph
    type: 'line', //Defines the type of graph
    data: { //Decides how the data (content of the graph will be)
        labels: timeArray, //Labels define the values of the x-axis (and can be altered at a later point/live)
        datasets: [ //Datasets refers to the different graphs and the data they contain
            {
                label: 'WaterLvl', //Label of dataset/graph 1
                data: waterLevelArray, //The dataArray that actually stores the data
                backgroundColor: [ //The background color of the graph (usually not in use)
                    'rgba(255, 99, 132, 0.2)'
                ],
                borderColor: [ //The border color of the graph (the color of the actual line)
                    'rgba(255, 99, 12, 1)'
                ],
                borderWidth: 1, //The width of the graph line
                fill: false
            },

            {
                label: 'WaterP', //Label of dataset/graph 1
                data: waterPumpPowerArray, //The dataArray that actually stores the data
                backgroundColor: [ //The background color of the graph (usually not in use)
                    'rgb(127,160,218)'
                ],
                borderColor: [ //The border color of the graph (the color of the actual line)
                    'rgb(8,112,194)'
                ],
                borderWidth: 1, //The width of the graph line
                fill: false
            },

            {
                label: 'Soil', //Label of dataset/graph 1
                data: soilHumidityArray, //The dataArray that actually stores the data
                backgroundColor: [ //The background color of the graph (usually not in use)
                    'rgb(84,207,96)'
                ],
                borderColor: [ //The border color of the graph (the color of the actual line)
                    'rgb(10,127,21)'
                ],
                borderWidth: 1, //The width of the graph line
                fill: false
            },

            {
                label: 'Lux', //Label of dataset/graph 1
                data: luxArray, //The dataArray that actually stores the data
                backgroundColor: [ //The background color of the graph (usually not in use)
                    'rgb(240,173,255)'
                ],
                borderColor: [ //The border color of the graph (the color of the actual line)
                    'rgb(182,97,193)'
                ],
                borderWidth: 1, //The width of the graph line
                fill: false
            },

            {
                label: 'Temp', //Label of dataset/graph 1
                data: temperatureArray, //The dataArray that actually stores the data
                backgroundColor: [ //The background color of the graph (usually not in use)
                    'rgb(255,194,104)'
                ],
                borderColor: [ //The border color of the graph (the color of the actual line)
                    'rgb(178,124,13)'
                ],
                borderWidth: 1, //The width of the graph line
                fill: false
            },

            {
                label: 'CO2', //Label of dataset/graph 1
                data: CO2Array, //The dataArray that actually stores the data
                backgroundColor: [ //The background color of the graph (usually not in use)
                    'rgb(255,247,99)'
                ],
                borderColor: [ //The border color of the graph (the color of the actual line)
                    'rgb(186,160,3)'
                ],
                borderWidth: 1, //The width of the graph line
                fill: false
            },

            {
                label: 'pH', //Label of dataset/graph 1
                data: pHArray, //The dataArray that actually stores the data
                backgroundColor: [ //The background color of the graph (usually not in use)
                    'rgb(187,255,99)'
                ],
                borderColor: [ //The border color of the graph (the color of the actual line)
                    'rgb(100,123,20)'
                ],
                borderWidth: 1, //The width of the graph line
                fill: false
            },
            {
                label: 'Air', //Label of dataset/graph 1
                data: airHumidityArray, //The dataArray that actually stores the data
                backgroundColor: [ //The background color of the graph (usually not in use)
                    'rgb(232,99,255)'
                ],
                borderColor: [ //The border color of the graph (the color of the actual line)
                    'rgb(157,8,165)'
                ],
                borderWidth: 1, //The width of the graph line
                fill: false
            }
        ]
    },
    options: {
        scales: {
            yAxes: [{
                ticks: {
                    beginAtZero: true //Keep this true to begin at zero in the graph
                }
            }]
        },
        responsive: true,
        maintainAspectRatio: false
    }
});