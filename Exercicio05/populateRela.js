const fs = require("fs");
const _ = require('lodash');

const BASE_FILE_NAME1 = 'Vida_16X<nBlk>';
const NBLK_OPTIONS = [1, 2, 4, 8, 12, 16, 20, 24];


const GetRawResults = (nBlk_options, filename) => ({
    part1: _.map(
        nBlk_options,
        (nBlk) => require('./' + filename.replace('<nBlk>', nBlk))
    ),
        part2: _.map(
            nBlk_options,
            (nBlk) => require('./' + filename.replace('<nBlk>', nBlk) + '_part2')
        ),
});

const TransformResults = _.partial(_.map, _,
    _.flow(
        _.partial(_.keyBy, _, (res) => res.tam),
        _.partial(_.mapValues,
            _,
            (result, key, results) => ({
                timeLoop: result.tempos.comp,
                timeTotal: result.tempos.tot,
                speedup: results[1].tempos.comp / result.tempos.comp,
            })
        )
    )
);

const DoReplace = (rela, name, nBlk, val) => rela.replace(`${name}-${nBlk} `, val);
const DoNiceReplace = (part, where, what, field) => _.partial(
            _.reduce,
            results['part' + part],
            (rela, val, key) => DoReplace(rela, where + what + part, key, val.tempos[field]),
            _)

const PopulateRela = (num, input, output) => (results) =>
    _.flow(
        () => fs.readFileSync(input, 'utf-8'),
        // part1
        DoNiceReplace(1, 'cpu', 'T', 'comp'),
        DoNiceReplace(1, 'cpu', 'T', 'tot'),
        DoNiceReplace(1, 'gpu', 'T', 'comp'),
        DoNiceReplace(1, 'gpu', 'T', 'tot'),
        // part2
        DoNiceReplace(2, 'cpu', 'T', 'comp'),
        DoNiceReplace(2, 'cpu', 'T', 'tot'),
        DoNiceReplace(2, 'gpu', 'T', 'comp'),
        DoNiceReplace(2, 'gpu', 'T', 'tot'),
        (rela) => fs.writeFileSync(output, rela),
        () => results
    )();

// Graph creation
const plotly = require('plotly')("ghust1995", "U0S5LTwnNH84fibf8Bdz")

const imgOpts = {
    format: 'png',
    width: 1000,
    height: 500,
};

//main
const print = res => {
    console.log(res);
    return res;
}

const DoGraphFromResults = (filename, part, mapX, mapY) => _.flow(
    (results) => ([{
        x: _.map(results['part' + part].cpu, mapX),
        y: _.map(results['part' + part].cpu, mapY),
        type: 'scatter',
        name: 'CPU',
    },
    {
        x: _.map(results['part' + part].gpu, mapX),
        y: _.map(results['part' + part].gpu, mapY),
        type: 'scatter',
        name: 'GPU',
    }]),
    print,
    // to figure
    (graphData) => ({
        data: graphData,
        options: {
            filename: filename,
            fileopt: 'overwrite',
        },
        layout: {},
    }),
    (figure) => plotly.getImage(figure, imgOpts, (error, imageStream) => {
        if (error) return console.log(error);
        imageStream.pipe(fs.createWriteStream(filename + '.png'));
    })
);

const doMainThing = (filename, relafile, num, graphname) => _.flow(
    () => GetRawResults(THREAD_OPTIONS, filename),
    TransformResults,
    PopulateRela(num, relafile),
    DoGraphFromResults(graphname, 1, (r) ),
    DoGraphFromResults(graphname),
    DoGraphFromResults(graphname),
    DoGraphFromResults(graphname),
)();

doMainThing(BASE_FILE_NAME1, './rela_raw.md', 1, 'speedup-no-opt');
doMainThing(BASE_FILE_NAME2, './rela.md', 2, 'speedup-o3');
