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
    let oneThreadTime = resultsBySize[size].find((res) => res.threads === 1).tempos.comp;
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
const plotly = require('plotly')("ghust1995", "U0S5LTwnNH84fibf8Bdz")
const _ = require('lodash');

let graphData =_.map(resultsBySize, (results, board_size) => {
    results.sort((a, b) => a.threads - b.threads)
    return {
        x: THREAD_OPTIONS,
        y: results.map((result) => result.speedup || 1),
        type: "scatter",
        name: `${board_size}x${board_size}`,
    };
});

let graphOptions = {
    filename: 'ce265ex02-speedup', 
    fileopt: 'overwrite',
};

let graphLayout = {
    title: 'Speedup Pelo Numero de Threads para Diferentes Tamanhos De Tabuleiro no Jogo da Vida',
}

let figure = { 
    data: graphData,
    options: graphOptions,
    layout: graphLayout,
};

{ // Plot Given figure
    let imgOpts = {
        format: 'png',
        width: 1000,
        height: 500,
    };
    plotly.getImage(figure, imgOpts, function (error, imageStream) {
        if (error) return console.log (error);

        var fileStream = fs.createWriteStream('speedup.png');
        imageStream.pipe(fileStream);
    });
}