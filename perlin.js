
function randRange(min, max){
    return Math.round(Math.random() * (max - min) + min);
}

class Vec2 {
    constructor(x, y){
        this.x = x;
        this.y = y;
    }

    dot(other){
        return other.x * this.x + other.y * this.y;
    }
}

class Vec3 {
    constructor(x, y, z){
        this.x = x;
        this.y = y;
        this.z = z;
    }

    dot(other){
        return other.x * this.x + other.y * this.y + other.z * this.z;
    }
}

class Perlin {
    constructor(){
        this.permutation_table = [];
        this.frequency = 0.003;
        this.persistence = 0.65;
    }

    setFrequency(f){
        this.frequency = f;
    }

    setPersistence(p){
        this.persistence = p;
    }

    init(){
        for (let i = 0; i < 256; i++){
            this.permutation_table.push(i);
        }
    }

    shuffle(){
        let p = [];
        while (this.permutation_table.length > 0){
            let i = randRange(0, this.permutation_table.length - 1);
            p.push(this.permutation_table[i]);
            this.permutation_table.splice(i, 1);
        }
        let n = p.length;
        for (let i = 0; i < n; i++){
            p.push(p[i]);
        }
        this.permutation_table = p;
    }

    fade(t){
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    lerp(t, a0, a1){
        return a0 + t * (a1 - a0);
    }
    
    getGradient1D(x){
        let i = this.permutation_table[x] & 1;
        if (i == 0){
            return 1.0;
        else {
            return  -1.0;
        }
    }

    getGradient2D(x, y){
        let i = this.permutation_table[this.permutation_table[x] + y] & 3;
        if (i == 0){
            return new Vec2(1, 1);
        } else if (i == 1){
            return new Vec2(1, -1);
        } else if (i == 2){
            return new Vec2(-1, 1);
        } else {
            return new Vec2(-1, -1);
        }
    }
        
    noise1D(x){
        let noiseGridX = Math.floor(x) & 255;
        
        let xf = x - Math.floor(x);
        
        let u = this.fade(xf);
        
        let left = xf;
        let right = xf - 1.0;
        
        let n = this.lerp(u, left * this.getGradient1D(noiseGridX), right * this.getGradient1D(noiseGridX + 1));
        
        n += 1.0;
        n /= 2.0;
        
        return n;

    noise2D(x, y){
        let noiseGridX = Math.floor(x) & 255;
        let noiseGridY = Math.floor(y) & 255;

        let xf = x - Math.floor(x);
        let yf = y - Math.floor(y);

        let u = this.fade(xf);
        let v = this.fade(yf);

        let tl = new Vec2(xf, yf - 1.0);
        let tr = new Vec2(xf - 1.0, yf - 1.0);
        let bl = new Vec2(xf, yf);
        let br = new Vec2(xf - 1.0, yf);

        let c0 = this.lerp(u, bl.dot(this.getGradient2D(noiseGridX, noiseGridY)), br.dot(this.getGradient2D(noiseGridX + 1, noiseGridY)));
        let c1 = this.lerp(u, tl.dot(this.getGradient2D(noiseGridX, noiseGridY + 1)), tr.dot(this.getGradient2D(noiseGridX + 1, noiseGridY + 1)));

        let n = this.lerp(v, c0, c1);

        n += Math.sqrt(2);
        n /= (2 * Math.sqrt(2));

        return n;
    }
        
    octaves1D(x, num_octaves){
        let n = 0;
        let amplitude = 1;
        let frequency = this.frequency;
        let max = 0;
        
        for (let o = 0; o < num_octaves; o++){
            n += amplitude * this.noise1D(x * frequency);
            max += amplitude;
            amplitude *= this.persistence;
            frequency *= 2;
        }

        n /= max;
        return n;
    }

    octaves2D(x, y, num_octaves){
        let n = 0;
        let amplitude = 1;
        let frequency = this.frequency;
        let max = 0;

        for (let o = 0; o < num_octaves; o++){
            n += amplitude * this.noise2D(x * frequency, y * frequency);
            max += amplitude;
            amplitude *= this.persistence;
            frequency *= 2;
        }

        n /= max;
        return n;
    }

    getGradient3D(x, y, z){
        let i = this.permutation_table[this.permutation_table[this.permutation_table[x] + y] + z] & 3;
        if (i == 0){
            return new Vec3(1, 1, -1);
        } else if (i == 1){
            return new Vec3(1, -1, -1);
        } else if (i == 2){
            return new Vec3(-1, 1, -1);
        } else if (i == 3){
            return new Vec3(-1, -1, -1);
        }else if (i == 4){
            return new Vec3(1, 1, 1);
        } else if (i == 5){
            return new Vec3(1, -1, 1);
        } else if (i == 6){
            return new Vec3(-1, 1, 1);
        } else {
            return new Vec3(-1, -1, 1);
        }
    }

    noise3D(x, y, z){
        let noiseGridX = Math.floor(x) & 255;
        let noiseGridY = Math.floor(y) & 255;
        let noiseGridZ = Math.floor(z) & 255;

        let xf = x - Math.floor(x);
        let yf = y - Math.floor(y);
        let zf = z - Math.floor(z);

        let u = this.fade(xf);
        let v = this.fade(yf);
        let w = this.fade(zf);

        let ltl = new Vec3(xf, yf - 1.0, zf);
        let ltr = new Vec3(xf - 1.0, yf - 1.0, zf);
        let lbl = new Vec3(xf, yf, zf);
        let lbr = new Vec3(xf - 1.0, yf, zf);

        let c0 = this.lerp(u, lbl.dot(this.getGradient3D(noiseGridX, noiseGridY, noiseGridZ)), lbr.dot(this.getGradient3D(noiseGridX + 1, noiseGridY, noiseGridZ)));
        let c1 = this.lerp(u, ltl.dot(this.getGradient3D(noiseGridX, noiseGridY + 1, noiseGridZ)), ltr.dot(this.getGradient3D(noiseGridX + 1, noiseGridY + 1, noiseGridZ)));
        let d0 = this.lerp(v, c0, c1);

        let utl = new Vec3(xf, yf - 1.0, zf - 1.0);
        let utr = new Vec3(xf - 1.0, yf - 1.0, zf - 1.0);
        let ubl = new Vec3(xf, yf, zf - 1.0);
        let ubr = new Vec3(xf - 1.0, yf, zf - 1.0);

        let c2 = this.lerp(u, ubl.dot(this.getGradient3D(noiseGridX, noiseGridY, noiseGridZ + 1)), ubr.dot(this.getGradient3D(noiseGridX + 1, noiseGridY, noiseGridZ + 1)));
        let c3 = this.lerp(u, utl.dot(this.getGradient3D(noiseGridX, noiseGridY + 1, noiseGridZ + 1)), utr.dot(this.getGradient3D(noiseGridX + 1, noiseGridY + 1, noiseGridZ + 1)));
        let d1 = this.lerp(v, c2, c3);

        let n = this.lerp(w, d0, d1);

        n += Math.sqrt(3);
        n /= (2 * Math.sqrt(3));

        return n;
    }

    octaves3D(x, y, z, num_octaves){
        let n = 0;
        let amplitude = 1;
        let frequency = this.frequency;
        let max = 0;

        for (let o = 0; o < num_octaves; o++){
            n += amplitude * this.noise3D(x * frequency, y * frequency, z * frequency);
            max += amplitude;
            amplitude *= this.persistence;
            frequency *= 2;
        }

        n /= max;
        return n;
    }
}
