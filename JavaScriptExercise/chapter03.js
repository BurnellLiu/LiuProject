

window.onload = function (){
    createBingoCard();

    document.getElementById("reload").onclick = reloadClicked
};

/**
 * 链接点击回调函数
 * @returns {boolean} false
 */
function reloadClicked() {
    createBingoCard();
    return false;
}

function createBingoCard() {
    /*
        Bingo卡片规则:
        B列包含数字1-15
        I列包含数字16-30
        N列包含数字31-45
        G列包含数字46-60
        O列包含数字61-75
        并且不能重复
    */

    // 每一列
    for (var i = 0; i < 5; i++) {
        var numArray = createBingoCol(i);

        // 每一行
        for (var j = 0; j < 5; j++) {
            var index = i * 5 + j;

            if (index === 12)
                continue;

            var elem = document.getElementById("square" + index);
            elem.innerHTML = numArray[j];
            elem.className = "";
            elem.onmousedown = squareMouseDown;

        }
    }
}

/**
 * 点击卡片回调函数
 * @param evt
 */
function squareMouseDown(evt) {
    if (evt) { // 非IE浏览器
        var square = evt.target;
    }
    else { // IE浏览器
        var square = window.event.srcElement;
    }

    if (square.className == "") {
        square.className = "winningBG";
    }
    else {
        square.className = ""
    }
}

/**
 * 创建Bingo卡片的一列数字
 * @param {Number} colIndex 列索引
 * @return {Array} 随机数组， 长度为5， 不重复
 */
function createBingoCol(colIndex) {
    var bingoArray = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ,11, 12, 13 ,14, 15];
    for (var i = 0; i < bingoArray.length; i++) {
        bingoArray[i] = bingoArray[i] + colIndex * 15;
    }

    shuffle(bingoArray);

    return bingoArray.slice(0, 5);
}

/**
 * 数组洗牌
 * @param {Array} array 被洗牌的数组
 * @return undefined
 */
function shuffle(array) {
    for (var i = array.length-1; i > 0; i--) {
        var j = Math.floor(Math.random() * (i + 1));
        var temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}