'use strict';

const die = {
	init(o) {
		this.throw(o);
	},
	throw(o,d=false) {
		typeof Object.prototype.toString.call(o) === '[object String]' ? o = [o] : false;
		if (o.length) {
			var t = this;
			if (d) {
				o.forEach(function(oi, i) {
					try {
						if (i<d.length) {
							if (d[i].value>=1 && d[i].value<=6)
								t.set(oi, t.faces(d[i].value, d[i].dir));
						}
					}catch(err) {
						console.log(err);
					}finally { }
				});
			}else {
				try {
					o.forEach(oi => t.set(oi, t.faces(1 + Math.floor(Math.random() * 6), Math.floor(Math.random() * 2))));
				}catch(err) {
					console.log(err);
				}finally { }
			}
		}
	},
	faces(tf/* 1..6 */, o/* 0..3 */) {
		// from top face and orientation find side face and front face
		if (tf == 1) 
			var a = [2, 3, 5, 4];
		else if (tf == 2)
			var a = [1, 4, 6, 3];
		else if (tf == 3)
			var a = [1, 2, 6, 5];
		else if (tf == 4)
			var a = [1, 5, 6, 2];
		else if (tf == 5)
			var a = [1, 3, 6, 4];
		else if (tf == 6)
			var a = [2, 4, 5, 3];
		var sf=a[(0+o) & 3];
		var ff=a[(1+o) & 3];
		return {s:sf, f:ff, t:tf, d:o & 1}; // die face numerics + direction flag
	},
	dots(v,d) {
		// set a face dots based on die value 1-6 and roll orientation/direction 0 or 1
		var dt=[] // order doesn't matter
		switch(v) {
			case 1:
				dt.push('020');
				break;
			case 2:
				d ? dt.push('100', '003') : dt.push('001', '300');
				break;
			case 3:
				d ? dt.push('100', '020', '003') : dt.push('001', '020', '300');
				break;
			case 4:
				dt.push('100', '001', '300', '003');
				break;
			case 5:
				dt.push('100', '001', '020', '300', '003');
				break;
			case 6:
				d ? dt.push('100', '010', '001', '300', '030', '003') : dt.push('100', '001', '200', '002', '300', '003');
				break;
		}
		return dt;
	},
	set(o, face/* {s:, f:, t:, d:} */) {
		var sty = $(o).attr('style');
		var svgDoc = $(o)[0].contentDocument;
		// optimised die has no xml header
		//if (svgDoc instanceof XMLDocument) {
			// Get the document object for the SVG and set base fill colour based upon object tags style
			$('#diecube', svgDoc).attr('style', sty);
			// Reset all dots to object tags style (i.e. same fill)
			for (var f=1; f<=3; f++) { // for face groups 1...3
				for (var r=1; r<=3; r++) { // for face 1..3
					var rs = r.toString();
					for (var c=1; c<=3; c++) { // for face columns 1..3
						var cols = '000'; // all 0 except the dot to set in row
						cols = cols.substring(0, c-1) + rs + cols.substring(c);
						var fid = '#face' + f.toString() + '-' + cols;
						$(fid, svgDoc).attr('style', sty);
					}
				}
			}
			// set each faces fill based upon patten of dots
			if (typeof face != 'undefined' && face != null) { // set faces
				this.dots(face.s, !face.d).forEach(e => $('#face1-'+e, svgDoc).css('fill', '#000000'));
				this.dots(face.f, face.d).forEach(e => $('#face2-'+e, svgDoc).css('fill', '#000000'));
				this.dots(face.t, !face.d).forEach(e => $('#face3-'+e, svgDoc).css('fill', '#000000'));
			}
		//}else
		//	console.log('SVG XMLDocument not found?');
	}
}; // die
