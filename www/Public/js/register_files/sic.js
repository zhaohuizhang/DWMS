var SIC = {};
SIC.Selector = new function() {
	var dataObj = {};
	var createOptions = function(sel, code, d) {
		var root = dataObj[code];
		if (root && root.children) {
			if (root.children.length > 1) {
				sel.options[0] = new Option("--- 请选择 ---", "");
			}
			$.each(root.children,function(index,value) {
				var child = dataObj[value];
				sel.options[sel.options.length] = new Option(child.name, child.code);
			});
		}
	};
	this.attach = function(firstSel, secondSel, thirdSel, forthSel) {
		var f = {};
		f.selectFirst = function(code, flag) {
			if (code) {
				firstSel.value = code;
			}
			secondSel.options.length = 0;
			thirdSel.options.length = 0;
			forthSel.options.length = 0;
			createOptions(secondSel, firstSel.value);
			if (secondSel.value) {
				this.selectSecond(secondSel.value, flag);
			}
		};
		f.selectSecond = function(code, flag) {
			if (code) {
				secondSel.value = code;
			}
			thirdSel.options.length = 0;
			forthSel.options.length = 0;
			createOptions(thirdSel, secondSel.value);
			if (thirdSel.value) {
				this.selectThird(thirdSel.value, flag);
			}
		};
		f.selectThird = function(code, flag) {
			if (code) {
				thirdSel.value = code;
			}
			forthSel.options.length = 0;
			createOptions(forthSel, thirdSel.value);
		};
		f.selectForth = function(code, flag) {
			if (code == "1") {
				return;
			}
			if (code) {
				forthSel.value = code;
			}
			if (typeof flag == "undefined" || flag == true) {
				var forth = dataObj[code];
				if (!forth) {
					return;
				}
				var pcode = forth.parent;
				var ppcode = dataObj[pcode].parent;
				var pppcode = ppcode?dataObj[ppcode].parent:null;
				if (pcode == "1") {
					f.selectFirst(code, true);
				} else {
					if (ppcode == "1") {
						f.selectFirst(pcode, true);
						f.selectSecond(code, true);
					} else if (pppcode == "1") {
						f.selectFirst(ppcode, true);
						f.selectSecond(pcode, true);
						f.selectThird(code, true);
					} else {
						f.selectFirst(pppcode, true);
						f.selectSecond(ppcode, true);
						f.selectThird(pcode, true);
						f.selectForth(code, false);
					}
				}
			}
		};
		f.initSelectedCodes = function(code) {
			f.selectForth(code, true);
		};
		f.getSelectedCodes = function() {
			return [ firstSel.value, secondSel.value ];
		};
		f.getDivisionText = function(j) {
			var i = [ firstSel, secondSel ].map(function(k) {
				if (k.selectedIndex >= 0) {
					return k.options[k.selectedIndex].text;
				}
			});
			return i.join(j || "");
		};

		createOptions(firstSel, "1");
		$(firstSel).change(function(event) {
			f.selectFirst(event.target.value);
		});
		$(secondSel).change(function(event) {
			f.selectSecond(event.target.value);
		});
		$(thirdSel).change(function(event) {
			f.selectThird(event.target.value);
		});
		return f;
	};

	this.init = function(sicData) {
		dataObj["1"] = {
			name : "dataObj",
			code : "1",
			children : []
		};

		for ( var code in sicData) {
			var cityInfo = sicData[code], pcode = cityInfo[1] || "1";
			dataObj[code] = {
				name : cityInfo[0],
				code : code,
				parent : cityInfo[1]
			};
			if (dataObj[pcode]) {
				if (!dataObj[pcode]["children"]) {
					dataObj[pcode]["children"] = [];
				}
				dataObj[pcode]["children"].push(code);
			}
		}
	}
}
SIC.Selector.sicData = {};
$.ajax({
    url: "../app/sic.do",
    dataType: 'json',
    async: false,
    cache: false,
    success: function(data) {
        SIC.Selector.sicData = data;
    }
});

SIC.Selector.init(SIC.Selector.sicData);