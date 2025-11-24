#pragma once

// GLSL prep
vec2 FC{static_cast<float>(w), static_cast<float>(h)};
const vec2 r{static_cast<float>(width), static_cast<float>(height)};
vec4 output;

//////////////////////////////
// https://x.com/XorDev/status/1894123951401378051
vec2 p = (FC * 2. - r) / r.y(), l, i, 
     v = p * (l += 4. - 4. * abs(.7 - dot(p, p)));
for (; i.y()++ < 8.; output += (sin(v.xyyx()) + 1.) * abs(v.x() - v.y()))
  v += cos(v.yx() * i.y() + i + t) / i.y() + .7;
output = tanh(5. * exp(l.x() - 4. - p.y() * vec4{-1.0f, 1.0f, 2.0f, 0.0f}) / output);
//////////////////////////////
