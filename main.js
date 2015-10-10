var Elligator2Curve25519 = function (curve) {
	var A = curve.a;
	var AA = A.redMul(A);
	var mA = A.redNeg();
	var u = curve.two;
	var mu = u.redNeg();
	var p12 = curve.p.subn(1).divn(2);

	var encode = function (point, alternative) {
		var x = point.getX().toRed(curve.red);
		var xA = x.redAdd(A);
		var muxA = mu.redMul(xA);

		if (!x.cmp(mA) || x.redMul(muxA).redPow(p12).cmpn(1) === 1) {
			return null;
		}

		var r = (alternative ? xA.redMul(mu.redMul(x).redInvm()) : x.redMul(muxA.redInvm())).redSqrt();

		if (r.cmp(p12) === 1) {
			r = r.redNeg();
		}

		var representative = r.toArray();

		var result = new Uint8Array(32);

		result.set(representative, 32 - representative.length);

		return result;
	};

	var decode = function (representative) {
		var r = new curve.zero.constructor(representative).toRed(curve.red);
		var rr = r.redMul(r);
		var urr1 = curve.one.redAdd(u.redMul(rr));

		if (!urr1.cmpn(0) || !AA.redMul(u).redMul(rr).cmp(urr1.redMul(urr1))) {
			return null;
		}

		var v = mA.redMul(urr1.redInvm());
		var vv = v.redMul(v);
		var e = vv.redMul(v).redAdd(A.redMul(vv)).redAdd(v).redPow(p12);
		var x = e.cmpn(1) ? v.redNeg().redSub(A) : v;

		var point = curve.point(x, curve.one);
		var alternative = e.cmpn(1) == 0;

		return [point, alternative];
	};

	this.encode = encode;
	this.decode = decode;
};
