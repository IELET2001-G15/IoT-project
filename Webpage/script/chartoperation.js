var ctx = document.getElementById('myChart').getContext('2d'); //Defines the basic graphic element of the graph

// Graph live data
var myLineChart = new Chart(ctx, {
    type: 'line',
    data: {

        // x-axis of the graph
        labels: timeArray,
        datasets: [
            {   // Water level graph
                label: 'WaterLvl',
                data: waterLevelArray,
                backgroundColor: [
                    'rgba(255, 99, 132, 0.2)'
                ],
                borderColor: [
                    'rgba(255, 99, 12, 1)'
                ],
                borderWidth: 1,
                fill: false
            },

            {   // Water pump power graph
                label: 'WaterP',
                data: waterPumpPowerArray,
                backgroundColor: [
                    'rgb(127,160,218)'
                ],
                borderColor: [
                    'rgb(8,112,194)'
                ],
                borderWidth: 1,
                fill: false
            },

            {   // Soil humidity graph
                label: 'Soil',
                data: soilHumidityArray,
                backgroundColor: [
                    '#009A44'
                ],
                borderColor: [
                    'rgb(10,127,21)'
                ],
                borderWidth: 1,
                fill: false
            },

            {   // Lux graph
                label: 'Lux',
                data: luxArray,
                backgroundColor: [
                    'rgb(240,173,255)'
                ],
                borderColor: [
                    'rgb(182,97,193)'
                ],
                borderWidth: 1,
                fill: false
            },

            {   // Temperature graph
                label: 'Temp',
                data: temperatureArray,
                backgroundColor: [
                    'rgb(255,194,104)'
                ],
                borderColor: [
                    'rgb(178,124,13)'
                ],
                borderWidth: 1,
                fill: false
            },

            {   // CO2 graph
                label: 'CO2',
                data: CO2Array,
                backgroundColor: [
                    'rgb(255,247,99)'
                ],
                borderColor: [
                    'rgb(186,160,3)'
                ],
                borderWidth: 1,
                fill: false
            },

            {   // pH level graph
                label: 'pH',
                data: pHArray,
                backgroundColor: [
                    'rgb(187,255,99)'
                ],
                borderColor: [
                    'rgb(100,123,20)'
                ],
                borderWidth: 1,
                fill: false
            },
            {   // Air quality graph
                label: 'Air',
                data: airHumidityArray,
                backgroundColor: [
                    'rgb(232,99,255)'
                ],
                borderColor: [
                    'rgb(157,8,165)'
                ],
                borderWidth: 1,
                fill: false
            }
        ]
    },
    options: {
        scales: {
            yAxes: [{
                ticks: {
                    beginAtZero: true
                }
            }]
        },
        responsive: true,
        maintainAspectRatio: false
    }
});