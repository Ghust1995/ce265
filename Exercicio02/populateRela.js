const fs = require("fs");

const BASE_FILE_NAME = "output_test_"

//const THREAD_OPTIONS = [1, 2, 4, 8, 16];
const THREAD_OPTIONS = [1, 2, 4];

// Reading results
let results = []
THREAD_OPTIONS.forEach(function(num_threads) {
    let lala = require('./' + BASE_FILE_NAME + num_threads)
    results = results.concat(lala);
}, this);

// Organizing results
let resultsBySize = results.reduce((reduced, result) => {
    if(!reduced[result.tam]){
        reduced[result.tam] = []
    }
    reduced[result.tam].push(result);
    return reduced;
}, {});

// Populating rela
let rela = fs.readFileSync('./rela_raw.md', 'utf-8');
Object.keys(resultsBySize).forEach(size => {
    var oneThreadTime = resultsBySize[size].find((res) => res.threads === 1).tempos.comp;
    resultsBySize[size].forEach(val => {
        let tam = val.tam;
        let threads = val.threads;
        let timeTemplate = `time${tam}-${threads}`;
        rela = rela.replace(timeTemplate, val.tempos.comp)
        if(val.threads === 1) return;
        let speedupTemplate = `sup${tam}-${threads}`;
        let speedup = oneThreadTime/val.tempos.comp;
        rela = rela.replace(speedupTemplate, speedup);
        val.speedup = speedup;
    });
});

fs.writeFileSync('./rela.md', rela);

// Graph creation
const ChartjsNode = require('chartjs-node');
const _ = require('lodash');

let chartData =_.mapValues(resultsBySize, (results) => {
    results.sort((a, b) => a.threads - b.threads)
    return results.map((result) => result.speedup || 1);
});

console.log(chartData);

var chart = new ChartjsNode(500, 500);

chartOptions = {
    data: {
        labels: THREAD_OPTIONS,
        datasets: chartData,
    }
};
chart.drawChart(chartOptions)
    .then(() => chart.writeImageToFile('image/png', './speedup.png'))
