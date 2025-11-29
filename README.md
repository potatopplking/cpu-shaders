# cpu shaders

Based on [Mr.Zozin's](https://gist.github.com/rexim) [awesome youtube video](https://www.youtube.com/watch?v=xNX9H_ZkfNE). Source code [here](https://gist.github.com/rexim/ef86bf70918034a5a57881456c0a0ccf#file-plasma-cpp).


```bash
$ ffmpeg -i output-%d.ppm -r 60 output.mp4
```

## TODO

- [ ] Add tests
    - [x] basic manual test
    - [ ] tests for static/compiled version, several shaders
- [ ] Use templated vector + specializations
- [ ] split into modules / classes
- [ ] try another shader
