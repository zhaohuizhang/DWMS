var SR = {};
SR.CitySelector = new function() {
	var dataObj = {};

	var createOptions = function(sel, code, d) {
		var root = dataObj[code];
		if (root && root.children) {
			if (root.children.length > 1) {
				sel.options[0] = new Option("--- 请选择 ---", "");
			}
			$.each(root.children, function(index, value) {
				var child = dataObj[value];
				sel.options[sel.options.length] = new Option(child.name, child.code);
			});
		}
	};
	this.attach = function(provSel, citySel, distSel) {
		var f = {};
		f.selectProv = function(code, flag) {
			if (code) {
				provSel.value = code;
			}
			citySel.options.length = 0;
			distSel.options.length = 0;
			createOptions(citySel, provSel.value);
			if (citySel.value) {
				this.selectCity(citySel.value, flag);
			}
		};
		f.selectCity = function(code, flag) {
			if (code) {
				citySel.value = code;
			}
			distSel.options.length = 0;
			createOptions(distSel, citySel.value);
		};
		f.selectDist = function(code, flag) {
			if (code == "1") {
				return;
			}
			if (code) {
				distSel.value = code;
			}
			if (typeof flag == "undefined" || flag == true) {
				var dist = dataObj[code];
				if (!dist) {
					return;
				}
				var pcode = dist.parent;
				var ppcode = dataObj[pcode].parent;
				if (pcode == "1") {
					f.selectProv(code, true);
				} else {
					if (ppcode == "1") {
						f.selectProv(pcode, true);
						f.selectCity(code, true);
					} else {
						f.selectProv(ppcode, true);
						f.selectCity(pcode, true);
						f.selectDist(code, false);
					}
				}
			}
		};
		f.initSelectedCodes = function(code) {
			f.selectDist(code, true);
		};
		f.getSelectedCodes = function() {
			return [ provSel.value, citySel.value ];
		};
		f.getDivisionText = function(j) {
			var i = [ provSel, citySel ].map(function(k) {
				if (k.selectedIndex >= 0) {
					return k.options[k.selectedIndex].text;
				}
			});
			return i.join(j || "");
		};
		createOptions(provSel, "1");
		$(provSel).change(function(event) {
			f.selectProv(event.target.value);
		});
		$(citySel).change(function(event) {
			f.selectCity(event.target.value);
		});
		return f;
	};

	this.init = function(cityData) {
		dataObj["1"] = {
			name : "国家",
			code : "1",
			children : []
		};
		for ( var code in cityData) {
			if (isNaN(parseInt(code))) {
				continue;
			}
			var cityInfo = cityData[code], pcode = cityInfo[1] || "1";
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
	};
	
	this.getName = function(code) {
		if ("000000" == code) {
			return "国家";
		}
		var obj = dataObj[code];
		if (obj) {
			return obj.name;
		} else {
			return "未知";
		}
	};
};
SR.CitySelector.cityData = {};
$.ajax({
    url: "../app/prov.do",
    dataType: 'json',
    async: false,
    cache: false,
    success: function(data) {
        SR.CitySelector.cityData = data;
    }
});

SR.CitySelector.init(SR.CitySelector.cityData);