const fs = require("fs");
const _ = require('lodash');

const BASE_FILE_NAME = 'Out_Crivo_X_OMP';
const THREAD_OPTIONS = [1, 2, 4];//, 8, 12, 16, 20, 24];
//const THREAD_OPTIONS = [1, 2, 4, 8, 12, 16, 20, 24];


const GetRawResults = (thread_options, filename) => {
    return _.map(
        thread_options, 
        (num_threads) => require('./' + filename.replace('X', num_threads))
    );
}

const TransformResults = _.flow(
    _.partial(_.keyBy, _, (res) => res.threads),
    _.partial(_.mapValues,
        _,
        (result, key, results) => ({
            time: result.tempos.comp,
            speedup: results[1].tempos.comp / result.tempos.comp,
        })
    )
);

const DoReplace = (rela, name, threads, val) => rela.replace(`${name}-${threads} `, val);

const PopulateRela = (results) => {
    _.flow(
        () => fs.readFileSync('./rela_raw.md', 'utf-8'),

        _.partial(
            _.reduce,
            results,
            (rela, val, key) => DoReplace(rela, 'time1', key, val.time),
            _),

        _.partial(
            _.reduce,
            results,
            (rela, val, key) => DoReplace(rela, 'sup1', key, val.speedup),
            _),

        (rela) => fs.writeFileSync('./rela.md', rela)
    )();
}

/*

// Graph creation
const plotly = require('plotly')("ghust1995", "U0S5LTwnNH84fibf8Bdz")

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
    //title: 'Speedup Pelo Numero de Threads para Diferentes Tamanhos De Tabuleiro no Jogo da Vida',
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
*/

//main
const print = res => {
    console.log(res);
    return res;
}
_.flow(
    () => GetRawResults(THREAD_OPTIONS, BASE_FILE_NAME),
    TransformResults,
    PopulateRela
)();