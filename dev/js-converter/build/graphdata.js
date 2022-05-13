"use strict";

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

///////////////////////////////////////////////////////////////////////////////
// File: graphdata.js
// Author : Yann Thoma
// Date   : 20.08.2021
//
// Description : This file embeds classes used by the graph rendering engine.
//               Filling a GraphFullData object allows to give all information
//               required to draw a canvas.
//
///////////////////////////////////////////////////////////////////////////////

// A list of percentiles.
// Usually we should have 7 items in it:
// Percentiles 5, 10, 25, 50, 75, 90, 95
//
// Each item shall be a GraphPercentileData
//
var GraphPercentileDataList = function () {
    function GraphPercentileDataList() {
        _classCallCheck(this, GraphPercentileDataList);

        this.plist = [];
        this.isValid = false;
    }

    _createClass(GraphPercentileDataList, [{
        key: "isEmpty",
        value: function isEmpty() {
            return this.plist.length == 0;
        }
    }, {
        key: "size",
        value: function size() {
            return this.plist.length;
        }
    }, {
        key: "objat",
        value: function objat(index) {
            return this.plist[index];
        }
    }, {
        key: "append",
        value: function append(data) {
            this.plist.push(data);
        }
    }]);

    return GraphPercentileDataList;
}();

// A GraphFancyPoint represents a concentration value at a certain time
// It is not strictly required by graphing.js, but could be useful 
// maybe for a controller


var GraphFancyPoint = function () {
    function GraphFancyPoint(times, values) {
        _classCallCheck(this, GraphFancyPoint);

        this.times = times;
        this.values = values;
    }

    _createClass(GraphFancyPoint, [{
        key: "times",
        value: function times() {
            return this.times;
        }
    }, {
        key: "values",
        value: function values() {
            return this.values;
        }
    }]);

    return GraphFancyPoint;
}();

// GraphPredictionData embeds all information related to a prediction.
//
// It shall supply the times as a vector, and the values also as a vector
//
// Various methods allow to get statistical values of a specific index
// in time. Here it is hardcoded, but it should depend on the values
// stored in the object.
//


var GraphPredictionData = function () {
    function GraphPredictionData(time, value) {
        _classCallCheck(this, GraphPredictionData);

        this.time = time;
        this.value = value;
        // The indexes at which there are trough concentrations
        this.troughs = [6];
        // The indexes at which there are peaks
        this.peaks = [1, 4];
        this.isValid = false;
        this.selected = false;
    }

    _createClass(GraphPredictionData, [{
        key: "times",
        value: function times() {
            return this.time;
        }
    }, {
        key: "values",
        value: function values() {
            return this.value;
        }
    }, {
        key: "meanAt",
        value: function meanAt(index) {
            return 10;
        }
    }, {
        key: "timeAt",
        value: function timeAt(index) {
            return this.time[index];
        }
    }, {
        key: "valueAt",
        value: function valueAt(index) {
            return this.value[index];
        }
    }, {
        key: "aucAt",
        value: function aucAt(index) {
            return 20;
        }
    }, {
        key: "auc24At",
        value: function auc24At(index) {
            return 30;
        }
    }, {
        key: "cumulatedAucAt",
        value: function cumulatedAucAt(index) {
            return 40;
        }
    }, {
        key: "troughAt",
        value: function troughAt(index) {
            return 50;
        }
    }, {
        key: "peakAt",
        value: function peakAt(index) {
            return 60;
        }
    }, {
        key: "timeAfterDose",
        value: function timeAfterDose(index) {
            return 70;
        }
    }, {
        key: "cycleDuration",
        value: function cycleDuration(index) {
            return 80;
        }
    }]);

    return GraphPredictionData;
}();

// A GraphPercentileData embeds the percentile rank and
// a GraphPredictionData for its curve.
//


var GraphPercentileData = function GraphPercentileData() {
    _classCallCheck(this, GraphPercentileData);

    this.percentile = 0;
    this.predictionData = null;
};

// A GraphPredictive embeds a prediction, along with its percentiles.
// Typically, a typical patient curve is represented by a GraphPredictive,
// as well as an apriori and a posteriori curve.
//


var GraphPredictive = function GraphPredictive() {
    _classCallCheck(this, GraphPredictive);

    this.predictionData = new GraphPredictionData();
    this.percentilePairs = new GraphPercentileDataList();
};

// A GraphPredictionResult just embeds a GraphPredictive, for compatibility
// with Qt Objects


var GraphPredictionResult = function GraphPredictionResult() {
    _classCallCheck(this, GraphPredictionResult);

    this.predictive = new GraphPredictive();
};

// This class embeds information about what to show or not on the graph.
// graphing.js uses that to know whether some curves or annotations shall be
// displayed or not.


var GraphInformationSelection = function GraphInformationSelection() {
    _classCallCheck(this, GraphInformationSelection);

    this.presentPopulationPrediction = true;
    this.displayPopulationPrediction = true;

    this.presentAprioriPrediction = true;
    this.displayAprioriPrediction = true;

    this.presentAposterioriPrediction = true;
    this.displayAposterioriPrediction = true;

    this.presentPopulationPercentiles = true;
    this.displayPopulationPercentiles = true;

    this.presentAprioriPercentiles = true;
    this.displayAprioriPercentiles = true;

    this.presentAposterioriPercentiles = true;
    this.displayAposterioriPercentiles = true;

    this.presentPossibleAdjustments = true;
    this.displayPossibleAdjustments = true;

    this.presentSelectedAdjustments = true;
    this.displaySelectedAdjustments = true;

    this.presentMeasures = true;
    this.displayMeasures = true;

    this.presentTargets = true;
    this.displayTargets = true;

    this.displayCovariateChange = true;
    this.displayCurrentTime = true;

    this.perc50 = true;
    this.perc25_75 = true;
    this.perc10_90 = true;
    this.perc5_95 = true;
};

// For the graph, the only required information about a covariate is
// its modification date


var GraphCovariate = function GraphCovariate() {
    var date = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : new Date(Date.now() - 12 * 3600 * 1000);

    _classCallCheck(this, GraphCovariate);

    this.date = date;
};

// For a dosage, the graph only needs its start and end time


var GraphDosage = function GraphDosage() {
    _classCallCheck(this, GraphDosage);

    this.applied = new Date(Date.now() - 24 * 3600 * 1000);
    this.endtime = new Date(Date.now() + 24 * 3600 * 1000);
};

// This class represents a value in conjunction with a multiplier coefficient and a unit.
// For the web front end, the multiplier could stay at 1.


var GraphIdentifiableAmount = function GraphIdentifiableAmount(value, multiplier, unit) {
    _classCallCheck(this, GraphIdentifiableAmount);

    this.dbvalue = value;
    this.multiplier = multiplier;
    this.unitstring = unit;
};

// This class represents a measure, being a date and a concentration.
// The concentration embeds a value, a multiplier and a unit.


var GraphMeasure = function GraphMeasure(moment, value, multiplier, unit) {
    _classCallCheck(this, GraphMeasure);

    this.moment = moment;
    this.concentration = new GraphIdentifiableAmount(value, multiplier, unit);
};

// A GraphTargetMethod represents a type of target.
// The value shall be corresponding to the following enum type:
//
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
// UnknownTarget        //! Unknown or invalid target.


var GraphTargetMethod = function GraphTargetMethod() {
    var value = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : 0;

    _classCallCheck(this, GraphTargetMethod);

    this.value = value;
};

// This class represents a value in conjunction with a multiplier coefficient.
// For the web front end, the multiplier could stay at 1.


var GraphAmount = function GraphAmount(dbValue) {
    var multiplier = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : 1;

    _classCallCheck(this, GraphAmount);

    this.dbvalue = dbValue;
    this.multiplier = multiplier;
};

// A GraphTarget embeds information about a target.
// It consists of its type represented by a GraphTargetMethod, a best, min and max
// value.


var GraphTarget = function GraphTarget(type) {
    var cmin = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : 0;
    var cbest = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : 2;
    var cmax = arguments.length > 3 && arguments[3] !== undefined ? arguments[3] : 10;

    _classCallCheck(this, GraphTarget);

    this.type = new GraphTargetMethod(type);
    this.cbest = new GraphAmount(cbest, 1.0);
    this.cmin = new GraphAmount(cmin, 1.0);
    this.cmax = new GraphAmount(cmax, 1.0);
};

// This class represents a suggested adjustment.
// It shall just embed a GraphPredictioData


var GraphAdjustment = function GraphAdjustment() {
    _classCallCheck(this, GraphAdjustment);

    this.predictionData = new GraphPredictionData();
};

// This class represents all the suggested adjustments.
// It is simply a list of GraphAdjustments


var GraphAdjustments = function () {
    function GraphAdjustments() {
        _classCallCheck(this, GraphAdjustments);

        this.alist = [];
        this.isValid = false;
    }

    _createClass(GraphAdjustments, [{
        key: "append",
        value: function append(adj) {
            this.alist.push(adj);
        }
    }, {
        key: "isEmpty",
        value: function isEmpty() {
            return this.alist.length == 0;
        }
    }, {
        key: "size",
        value: function size() {
            return this.alist.length;
        }
    }, {
        key: "objat",
        value: function objat(index) {
            return this.alist[index];
        }
    }]);

    return GraphAdjustments;
}();

var GraphMouseArea = function GraphMouseArea() {
    _classCallCheck(this, GraphMouseArea);

    this.mouseX = -1;
    this.mouseY = -1;
    this.containsMouse = false;
    this.isMouseOver = false;
    this.tooltipX = -1;
    this.tooltipY = -1;
};

var GraphFullData = function () {
    function GraphFullData() {
        var scale = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : 1;

        _classCallCheck(this, GraphFullData);

        this.scale = scale;

        // The canvas shall be set before this object is used
        this.canvas = null;
        this.annotationsCanvas = null;
        this.clockCanvas = null;

        this.colors = ["#bfbfbf", // Unused
        "#3c4042", // popPrediction
        "#004375", // aprioriPrediction
        "#9abfe7", // Unused
        //"#C80000",    // aposterioriPrediction
        "#269043", // aposterioriPrediction
        "#e8a45c", // suggestedAdjustmentLegend
        "#8cda75", // aposterioriPercentilesLegend
        "#aaaeb2", // popPercentilesLegend
        "#9abfe7", // aprioriPercentilesLegend
        "black" // selectedAdjustmentLegend
        ];

        this.popcolors = ["#aaaeb2", "#D2D6DA", "#E0E2E6", "#EAEEF0"];
        this.aprcolors = ["#9abfe7", "#9AE8E2", "#B6F4F0", "#DEFEFC"];
        this.apocolors = ["#8cda75", "#8CECA6", "#B4FABA", "#D6FCDA"];
        this.adjcolors = ["#323232", "#969696", "#646464"];
        this.revcolors = [];

        // constant indices
        this.pop = 0;
        this.apr = 1;
        this.apo = 2;
        this.rev = 3;
        this.mea = 4;
        this.tar = 5;
        this.adj = 6;

        // Could be a const
        this.indices = ["pop", "apr", "apo", "rev", "mea", "tar", "adj"];

        this.nographtext = "Welcome to Tucuxi! Graphs of predicted concentrations will be shown here.";

        // The following variables are generated by the drawer,
        // So no need to set them prior to the rendering
        this.hasMeasure = false;
        this.hasPatientVariates = false;
        this.hasTargets = true;

        this.popercsP = new GraphPercentileDataList();
        this.aprpercsP = new GraphPercentileDataList();
        this.apopercsP = new GraphPercentileDataList();
        this.adjpercsP = new GraphPercentileDataList();

        this.popP = new GraphPredictionResult();
        this.aprP = new GraphPredictionResult();
        this.apoP = new GraphPredictionResult();
        this.adjP = new GraphPredictionResult();

        this.revP = new GraphAdjustments();

        this.measures = [];
        this.targets = [];
        this.pvars = [];
        this.dosages = [];

        //Chart properties
        this.scaleMax = 2e1;
        this.scaleMin = 2e-3;
        this.police = "sans-serif";

        if (scale == 1) {
            this.fontSize = "12px";
            this.axisTicksFontSize = "10px";
            this.tooltipFontSize = "10px";
        } else if (scale == 2) {
            this.fontSize = "24px";
            this.axisTicksFontSize = "20px";
            this.tooltipFontSize = "20px";
        } else if (scale == 4) {
            this.fontSize = "48px";
            this.axisTicksFontSize = "40px";
            this.tooltipFontSize = "40px";
        }
        this.yFactor = 1.0;
        this.date = new Date();

        this.maxX = 0;
        this.minX = 0;
        this.maxY = 0;
        this.minY = 0;
        this.xRatio = 0;
        this.yRatio = 0;

        this.highlightthresh = 15;

        this.currentPoints = [];
        this.closestPred = {};
        this.currentMeasure = null;
        this.currentDosage = null;

        this.antialiasing = true;

        this.isOffsetXEnabled = true;
        this.isOffsetYEnabled = true;

        //ToDo = remove default unit
        this.unit = "ug/l";
        this.unitefforder = 1;

        this.gInformationSelection = new GraphInformationSelection();

        this.timestart = Date.now();

        //Canvas margins
        this.leftMargin = 75 * this.scale;
        this.rightMargin = 75 * this.scale;
        this.topMargin = 75 * this.scale;
        this.bottomMargin = 75 * this.scale;

        this.targetTabIndex = -1;

        this.mArea = new GraphMouseArea();
    }

    _createClass(GraphFullData, [{
        key: "getViewRangeMin",
        value: function getViewRangeMin() {
            var newdate = new Date();return newdate.getTime() / 1000 - 24 * 3600;
        }
    }, {
        key: "getViewRangeMax",
        value: function getViewRangeMax() {
            var newdate = new Date();return newdate.getTime() / 1000;
        }

        // This function shall be called before printing the graph,
        // to precalculate values.

    }, {
        key: "updateChartDimensions",
        value: function updateChartDimensions(canvas) {

            if (this.scale <= 1.5) {
                this.fontSize = "12px";
                this.axisTicksFontSize = "10px";
                this.tooltipFontSize = "10px";
            } else if (this.scale <= 2.5) {
                this.fontSize = "24px";
                this.axisTicksFontSize = "20px";
                this.tooltipFontSize = "20px";
            } else if (this.scale <= 4.5) {
                this.fontSize = "48px";
                this.axisTicksFontSize = "40px";
                this.tooltipFontSize = "40px";
            }

            //Canvas margins
            this.leftMargin = 75 * this.scale;
            this.rightMargin = 75 * this.scale;
            this.topMargin = 75 * this.scale;
            this.bottomMargin = 75 * this.scale;

            this.topLeftX = this.leftMargin;
            this.topLeftY = this.topMargin;

            this.topRightX = canvas.width - this.rightMargin;
            this.topRightY = this.topMargin;

            this.bottomLeftX = this.leftMargin;
            this.bottomLeftY = canvas.height - this.bottomMargin;

            this.bottomRightX = canvas.width - this.rightMargin;
            this.bottomRightY = canvas.height - this.bottomMargin;

            this.plotWidth = this.topRightX - this.topLeftX;
            this.plotHeight = this.bottomLeftY - this.topLeftY;
        }
    }]);

    return GraphFullData;
}();

// This function should be called when a prediction is ready,
// to generate the .X and .Y data
// This function is also available in Chart.qml


function preparePrediction(prediction, predType) {
    if (!prediction.predictive.predictionData.isValid) {
        return;
    }
    prediction.X = prediction.predictive.predictionData.times();
    prediction.Y = prediction.predictive.predictionData.values();
    prediction.predictive.predictionData.displayTooltip = true;
    prediction.predictive.predictionData.closestPoint = {
        currentindex: 0
    };
}

// This function should be called when the percentiles are ready,
// to generate the .X and .Y arrays
// This function is also available in Chart.qml
function preparePercs(pairs) {
    if (!pairs.isValid) {
        return;
    }
    if (pairs.size() === 0) {
        return;
    }
    for (var i = 0; i < pairs.size(); ++i) {
        if (pairs.objat(i).isValid) {
            pairs.objat(i).X = pairs.objat(i).predictionData.times();
            pairs.objat(i).Y = pairs.objat(i).predictionData.values();
            pairs.objat(i).predictionData.closestPoint = {
                currentindex: 0
            };
        }
    }
}