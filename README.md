# MediaHandling [![Codacy Badge](https://api.codacy.com/project/badge/Grade/fd1f6eda59fd4bdbbe54c90bac7300d5)](https://www.codacy.com/manual/jonno85uk/mediahandling?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=jonno85uk/mediahandling&amp;utm_campaign=Badge_Grade)[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

This is a library solely intended for the use in the project [Chestnut](https://github.com/jonno85uk/chestnut)
by handling all media-file operations (properties/decoding/encoding).
This is done by abstracting the underlying media library with a common interface.

Currently, only the [FFmpeg](https://ffmpeg.org/) library is being used.

## Installation

### Dependencies

[fmt](https://github.com/fmtlib/fmt) and [FFmpeg](https://ffmpeg.org/) development headers and libraries

### Building

Change directory to the top-level of the project.

<pre><code>mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr/local/ -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install</code></pre>

### Regression Tests

Install [git-lfs](https://git-lfs.github.com/) and [google-test](https://github.com/google/googletest)
<pre><code>git-lfs fetch
git-lfs pull
</pre></code>

Change directory to RegressionTests
<pre><code>mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr/local/ -DCMAKE_BUILD_TYPE=Release ..
make</code></pre>

Change back to RegressionTests directory and run

<pre>build/mh_regression</pre>
