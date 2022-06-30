"use strict";

// Make a date from a tuberXpert date string.
// Can't use the new Date(dateStr). This version is not
// supported by the PDF exporter.
function makeDate(dateStr) {
    var dateTimeSplits = dateStr.split('T');
    var dateSplit = dateTimeSplits[0];
    var timeSplit = dateTimeSplits[1];
    var date = dateSplit.split('-')
    var time = timeSplit.split(':')
    return new Date(date[0], date[1], date[2], time[0], time[1], time[2])
}

// Add targets to the graph.
// Expect an array of targets:
// targets : [ target : [type, min, best, max], ... ]
function addTargets(graph, targets) {

    for (var target = 0; target < targets.length; target++) {

        var targetType = -1;

        // ResidualTarget = 0,  //! Targeted residual concentration, or targetted concentration at the end of a complete cycle.
        // PeakTarget,          //! Targeted peak concentration. WARNING: It does not correspond to the maximum of a cycle, but the value at a specific time after the intake.
        // MeanTarget,          //! Targeted mean concentration, currently unused.
        // AUCTarget,           //! Targeted AUC concentration, currently unused.
        // AUC24Target,           //! Targeted AUC concentration, currently unused.
        // CumulativeAUCTarget, //! Targeted cumulative AUC concentration.
        // AUCDividedByMicTarget, //! Soon to be supported target
        // AUC24DividedByMicTarget, //! Soon to be supported target
        // AUCOverMicTarget, //! Soon to be supported target
        // AUC24OverMicTarget, //! Soon to be supported target
        // PeakDividedByMicTarget, //! Soon to be supported target
        // TimeOverMicTarget, //! Soon to be supported target
        // ResidualDividedByMicTarget, //! Targeted residual concentration divided by MIC

        switch (targets[target][0]) {
            case "residual":
                targetType = 0;break;
            case "peak":
                targetType = 1;break;
            case "mean":
                targetType = 2;break;
            case "auc":
                targetType = 3;break;
            case "auc24":
                targetType = 4;break;
            case "cumulativeAuc":
                targetType = 5;break;
            case "aucDividedByMic":
                targetType = 6;break;
            case "auc24DividedByMic":
                targetType = 7;break;
            case "aucOverMic":
                targetType = 8;break;
            case "auc24OverMic":
                targetType = 9;break;
            case "peakDividedByMic":
                targetType = 10;break;
            case "timeOverMic":
                targetType = 11;break;
            case "residualDividedByMic":
                targetType = 12;break;
        }

        // Let's add target
        graph.targets.push(new GraphTarget(targetType, targets[target][1], targets[target][2], targets[target][3]));
    }
}

// Add times and values to the adjustment considering the start date.
function addPredictionData(adjustment, start, times, values) {
    for (var i = 0; i < times.length; i++) {
        adjustment.predictionData.time.push(times[i] * 3600 + start.getTime() / 1000);
        adjustment.predictionData.value.push(values[i]);
    }
    adjustment.predictionData.troughs.push(adjustment.predictionData.value.length - 1);
    return adjustment;
}

// We expect a tripple dimension array. that looks like:
// adjustments : [ adjustment : [ CycleData : [ start, times : [...], values : [....] ] ] ]
// The first array contain an array for each adjustment (1st beeing the best)
// The array of each adjustment is filled with array of cycle data.
// The array of a cycle data contains 3 elements: its starting time, its times and values as array.
// The start time and end time are time strings. The form must looks like: "2022-07-06T08:00:00".
function displayGraphs(graph, adjustments, starttime, endtime) {

    // Setting the time bounds
    graph.timestart = makeDate(starttime);
    graph.timeend = makeDate(endtime);

    graph.revP.isValid = true;
    graph.scale = 1;

    // For each adjustment
    for (var adjustment = 0; adjustment < adjustments.length; adjustment++) {

        var adj = new GraphAdjustment();

        // For each cycle data in the adjustment
        for (var cycleData = 0; cycleData < adjustments[adjustment].length; cycleData++) {
            adj = addPredictionData(adj, makeDate(adjustments[adjustment][cycleData][0]), // Starting date
            adjustments[adjustment][cycleData][1], // Times
            adjustments[adjustment][cycleData][2] // Values
            );
        }
        adj.predictionData.isValid = true;
        graph.revP.append(adj);

        // We display the second graph immediately after the first adjustment.
        // The second graph is only displaying the best adjustment.
        if (adjustment == 0) {
            var canvasBest = document.getElementById("canBestAdj");

            graph.updateChartDimensions(canvasBest);
            graph.canvas = canvasBest;
            graph.annotationsCanvas = canvasBest;
            graph.clockCanvas = canvasBest;

            drawGraph(graph);
        }
    }

    var canvasAll = document.getElementById("canAllAdj");

    graph.updateChartDimensions(canvasAll);
    graph.canvas = canvasAll;
    graph.annotationsCanvas = canvasAll;
    graph.clockCanvas = canvasAll;

    drawGraph(graph);
}