@page benchmark Benchmark

@tableofcontents

Below are tables comparing this library with the boost asio library and the sockets API.

## Comparison tables {#comparison-tables}

@htmlonly

<style type="text/css">
  .details_class > summary:first-of-type {
    list-style: inside disclosure-closed;
  }
  .details_class[open] > summary:first-of-type {
    list-style: inside disclosure-open;
  }
  .details_class > summary::before {
    content: none;
  }
  .details_class[open] > summary::before {
    content: none;
  }
</style>

@endhtmlonly

### Linux (Ubuntu 20.04) {#linux}

<details class="details_class" open>
<summary><b> Table </b></summary>

@note
Run on (16 X 3110 MHz CPU s) <br>
CPU Caches: <br>
&nbsp;&nbsp;&nbsp;&nbsp;L1 Data 48 KiB (x8) <br>
&nbsp;&nbsp;&nbsp;&nbsp;L1 Instruction 32 KiB (x8) <br>
&nbsp;&nbsp;&nbsp;&nbsp;L2 Unified 1280 KiB (x8) <br>
&nbsp;&nbsp;&nbsp;&nbsp;L3 Unified 18432 KiB (x1) <br>
Load Average: 2.22, 2.16, 1.02

| input string                                    | ipaddress | socket API | boost asio |
|:----------------------------------------------- | ---------:| ----------:| ----------:|
| `127.0.0.1`                                     |   8.76 ns |    11.8 ns |    14.2 ns |
| `192.168.0.1`                                   |   9.81 ns |    13.4 ns |    16.5 ns |
| `0.0.0.0`                                       |   7.80 ns |    9.84 ns |    12.8 ns |
| `100.64.0.0`                                    |   9.32 ns |    13.1 ns |    15.2 ns |
| `192.168.1.1`                                   |   9.82 ns |    13.8 ns |    16.2 ns |
| `127.239.0.1`                                   |   9.81 ns |    13.5 ns |    16.0 ns |
| `127.128.128.255`                               |   13.1 ns |    18.2 ns |    26.4 ns |
| `224.1.1.1`                                     |   8.84 ns |    11.6 ns |    14.2 ns |
| `2001:db8::1`                                   |   39.3 ns |    20.6 ns |    27.0 ns |
| `0001:0002:0003:0004:0005:0006:0007:0008%12345` |    103 ns |    -       |    60.4 ns |
| `::ffff:172.32.0.0`                             |   62.5 ns |    29.8 ns |    38.5 ns |
| `2002:ac1d:2d64::1`                             |   50.8 ns |    28.3 ns |    36.4 ns |
| `2001:0000:4136:e378:8000:63bf:3fff:fdd2`       |    102 ns |    43.5 ns |    53.9 ns |
| `2000::4136:e378:8000:63bf:3fff:fdd2`           |   93.0 ns |    45.7 ns |    55.0 ns |
| `2001:db8:0:0:1:0:0:1`                          |   68.3 ns |    22.0 ns |    29.4 ns |
| `fe80::1ff:fe23:4567:890a%31`                   |   69.5 ns |    -       |    1115 ns |

</details>

### MacOS {#macos}

<details class="details_class">
<summary><b> Table </b></summary>

@note
Run on (8 X 2000 MHz CPU s) <br>
CPU Caches: <br>
&nbsp;&nbsp;&nbsp;&nbsp;L1 Data 48 KiB <br>
&nbsp;&nbsp;&nbsp;&nbsp;L1 Instruction 32 KiB <br>
&nbsp;&nbsp;&nbsp;&nbsp;L2 Unified 512 KiB (x4) <br>
&nbsp;&nbsp;&nbsp;&nbsp;L3 Unified 6144 KiB <br>
Load Average: 2.70, 4.59, 7.36

| input string                                    | ipaddress | socket API | boost asio |
|:----------------------------------------------- | ---------:| ----------:| ----------:|
| `127.0.0.1`                                     |   11.7 ns |    38.4 ns |    44.4 ns |
| `192.168.0.1`                                   |   13.9 ns |    45.3 ns |    50.6 ns |
| `0.0.0.0`                                       |   10.0 ns |    30.8 ns |    35.9 ns |
| `100.64.0.0`                                    |   12.6 ns |    40.9 ns |    45.7 ns |
| `192.168.1.1`                                   |   13.7 ns |    47.1 ns |    48.9 ns |
| `127.239.0.1`                                   |   13.4 ns |    43.7 ns |    50.0 ns |
| `127.128.128.255`                               |   18.0 ns |    56.4 ns |    61.5 ns |
| `224.1.1.1`                                     |   11.5 ns |    38.8 ns |    43.6 ns |
| `2001:db8::1`                                   |   55.4 ns |    69.0 ns |    77.4 ns |
| `0001:0002:0003:0004:0005:0006:0007:0008%12345` |    200 ns |     250 ns |     241 ns |
| `::ffff:172.32.0.0`                             |   91.2 ns |     107 ns |     125 ns |
| `2002:ac1d:2d64::1`                             |   77.6 ns |     101 ns |     111 ns |
| `2001:0000:4136:e378:8000:63bf:3fff:fdd2`       |    174 ns |     229 ns |     227 ns |
| `2000::4136:e378:8000:63bf:3fff:fdd2`           |    158 ns |     184 ns |     192 ns |
| `2001:db8:0:0:1:0:0:1`                          |    115 ns |     121 ns |     131 ns |
| `fe80::1ff:fe23:4567:890a%31`                   |    118 ns |   40403 ns |   41033 ns |

</details>

### Windows {#windows}

<details class="details_class">
<summary><b> Table </b></summary>

@note
Run on (16 X 3110 MHz CPU s) <br>
CPU Caches: <br>
&nbsp;&nbsp;&nbsp;&nbsp;L1 Data 48 KiB (x8) <br>
&nbsp;&nbsp;&nbsp;&nbsp;L1 Instruction 32 KiB (x8) <br>
&nbsp;&nbsp;&nbsp;&nbsp;L2 Unified 1280 KiB (x8) <br>
&nbsp;&nbsp;&nbsp;&nbsp;L3 Unified 18432 KiB (x1)

| input string                                    | ipaddress | socket API | boost asio |
|:----------------------------------------------- | ---------:| ----------:| ----------:|
| `127.0.0.1`                                     |   8.57 ns |    29.7 ns |     214 ns |
| `192.168.0.1`                                   |   9.89 ns |    33.9 ns |     217 ns |
| `0.0.0.0`                                       |   8.00 ns |    28.9 ns |     207 ns |
| `100.64.0.0`                                    |   9.57 ns |    32.2 ns |     217 ns |
| `192.168.1.1`                                   |   10.2 ns |    32.3 ns |     221 ns |
| `127.239.0.1`                                   |   10.2 ns |    33.6 ns |     221 ns |
| `127.128.128.255`                               |   14.6 ns |    38.9 ns |     229 ns |
| `224.1.1.1`                                     |   9.02 ns |    26.9 ns |     215 ns |
| `2001:db8::1`                                   |   38.3 ns |    94.1 ns |     271 ns |
| `0001:0002:0003:0004:0005:0006:0007:0008%12345` |    105 ns |    -       |     436 ns |
| `::ffff:172.32.0.0`                             |   61.6 ns |    144 ns  |     313 ns |
| `2002:ac1d:2d64::1`                             |   51.0 ns |    137 ns  |     314 ns |
| `2001:0000:4136:e378:8000:63bf:3fff:fdd2`       |    104 ns |    275 ns  |     448 ns |
| `2000::4136:e378:8000:63bf:3fff:fdd2`           |   93.6 ns |    249 ns  |     421 ns |
| `2001:db8:0:0:1:0:0:1`                          |   66.6 ns |    192 ns  |     350 ns |
| `fe80::1ff:fe23:4567:890a%31`                   |   69.6 ns |    -       |     367 ns |

</details>

@htmlonly

<div class="cards">

<div class="card">
  <a href="module.html">
  <div class="card_container">
    <svg viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"> <g id="Arrow / Arrow_Circle_Left"> <path id="Vector" d="M11 9L8 12M8 12L11 15M8 12H16M21 12C21 7.02944 16.9706 3 12 3C7.02944 3 3 7.02944 3 12C3 16.9706 7.02944 21 12 21C16.9706 21 21 16.9706 21 12Z" stroke="#000000" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path> </g> </g></svg>
    <h2>Cpp Module</h2>
    <p>Back to cpp module.</p>
  </div>
  </a>
</div>

<div class="card">
  <a href="build.html">
  <div class="card_container">
    <svg viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"> <g id="Arrow / Arrow_Circle_Right"> <path id="Vector" d="M13 15L16 12M16 12L13 9M16 12H8M21 12C21 7.02944 16.9706 3 12 3C7.02944 3 3 7.02944 3 12C3 16.9706 7.02944 21 12 21C16.9706 21 21 16.9706 21 12Z" stroke="#000000" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path> </g> </g></svg>
    <h2>Build Guide</h2>
    <p>Learn how to build the library, unit tests and documentation locally for development purposes.</p>
  </div>
  </a>
</div>

</div>

@endhtmlonly
