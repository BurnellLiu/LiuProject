


window.onload = function() {
    var elem = document.getElementById("newLocation");
    elem.selectedIndex = 0;
    elem.onchange = jumpPage;

    elem = document.getElementById("months");
    elem.onchange = populateDays;
}

window.onUnload = function() {

};

function jumpPage() {
    var elem = document.getElementById("newLocation");
    var newPage = elem.options[elem.selectedIndex].value;
    if (newPage != "") {
        window.location = newPage;
    }
}

function populateDays() {
    var daysArray = [0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
    var monthStr = this.options[this.selectedIndex].value;

    if (monthStr != "") {
        var month = parseInt(monthStr);

        var elem = document.getElementById("days");
        elem.options.length = 0;
        for (var i = 0; i < daysArray[month]; i++) {
            elem.options[i] = new Option(i + 1);
        }
    }
}