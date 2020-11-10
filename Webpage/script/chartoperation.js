var ctx = document.getElementById('myChart').getContext('2d'); //Defines the basic graphic element of the graph

var myLineChart = new Chart(ctx, { //Defines the graph
    type: 'line', //Defines the type of graph
    data: { //Decides how the data (content of the graph will be)
        labels: timersArray, //Labels define the values of the x-axis (and can be altered at a later point/live)
        datasets: [ //Datasets refers to the different graphs and the data they contain
            {
                label: 'Water Level', //Label of dataset/graph 1
                data: waterLevelArray, //The dataArray that actually stores the data
                backgroundColor: [ //The background color of the graph (usually not in use)
                    'rgba(255, 99, 132, 0.2)'
                ],
                borderColor: [ //The border color of the graph (the color of the actual line)
                    'rgba(255, 99, 12, 1)'
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