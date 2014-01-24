function sqr(x) {
    return x * x
}

function dist2(v, w) {
    return sqr(v.x - w.x) + sqr(v.y - w.y)
}

function distToSegmentSquared(p, v, w) {
    var l2 = dist2(v, w);
    if (l2 == 0) return dist2(p, v);
    var t = ((p.x - v.x) * (w.x - v.x) + (p.y - v.y) * (w.y - v.y)) / l2;
    if (t < 0) return dist2(p, v);
    if (t > 1) return dist2(p, w);
    return dist2(p, {
        x: v.x + t * (w.x - v.x),
        y: v.y + t * (w.y - v.y)
    });
}

function distToSegment(p, v, w) {
    return Math.sqrt(distToSegmentSquared(p, v, w));
}

function pointFromLatLng(lng, lat) {
    pi = 3.1415926535;
    return {
        x: (lng / 180.0 * pi * 5490),
        y: (lat / 180.0 * pi * 6340)
    };
}

function distToPath(p, arr) {
    if (arr.length <= 0) return 0.0;
    // if(arr.length == 1) return dist2(p, arr[0]);
    shortest = dist2(p, arr[0]);
    for (var i = 0; i < arr.length - 1; i++) {
        cur = distToSegment(p, arr[i], arr[i + 1]);
        if (cur < shortest) {
            shortest = cur;
        }
    }
    return shortest;
}

function shortestDist(p_json, arr_json) {
    p = pointFromLatLng(p_json["lng"], p_json["lat"]);
    newarr = []
    for (var i = 0; i < arr_json.length; i++) {
        newarr[i] = pointFromLatLng(arr_json[i]["lng"], arr_json[i]["lat"]);
    }
    return distToPath(p, newarr);
}
