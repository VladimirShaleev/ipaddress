@page benchmark Benchmark

@tableofcontents

## Comparison table

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

Below is a table comparing this library with the boost asio library and sockets API.

<details class="details_class" open>
<summary><b> Linux (Ubuntu) </b></summary>

@note
Run on (16 X 3110 MHz CPU s) <br>
CPU Caches: <br>
&nbsp;&nbsp;&nbsp;&nbsp;L1 Data 48 KiB (x8) <br>
&nbsp;&nbsp;&nbsp;&nbsp;L1 Instruction 32 KiB (x8) <br>
&nbsp;&nbsp;&nbsp;&nbsp;L2 Unified 1280 KiB (x8) <br>
&nbsp;&nbsp;&nbsp;&nbsp;L3 Unified 18432 KiB (x1)

| input string                                    | ipaddress | socket API | boost asio |
| ----------------------------------------------- | ---------:| ----------:| ----------:|
| `127.0.0.1`                                     |   12.6 ns |    16.5 ns |    17.8 ns |
| `192.168.0.1`                                   |   14.5 ns |    19.6 ns |    20.8 ns |
| `0.0.0.0`                                       |   9.29 ns |    12.9 ns |    15.4 ns |
| `100.64.0.0`                                    |   11.6 ns |    16.9 ns |    20.7 ns |
| `192.168.1.1`                                   |   12.5 ns |    17.5 ns |    22.6 ns |
| `127.239.0.1`                                   |   12.3 ns |    17.9 ns |    21.6 ns |
| `127.128.128.255`                               |   17.9 ns |    23.1 ns |    25.6 ns |
| `224.1.1.1`                                     |   11.0 ns |    16.1 ns |    17.8 ns |
| `2001:db8::1`                                   |   57.7 ns |    24.6 ns |    36.4 ns |
| `0001:0002:0003:0004:0005:0006:0007:0008%12345` |    165 ns |    -       |    73.3 ns |
| `::ffff:172.32.0.0`                             |   97.7 ns |    40.7 ns |    50.5 ns |
| `2002:ac1d:2d64::1`                             |   74.4 ns |    38.4 ns |    43.9 ns |
| `2001:0000:4136:e378:8000:63bf:3fff:fdd2`       |    130 ns |    53.0 ns |    69.2 ns |
| `2000::4136:e378:8000:63bf:3fff:fdd2`           |    119 ns |    58.3 ns |    68.1 ns |
| `2001:db8:0:0:1:0:0:1`                          |   89.7 ns |    27.7 ns |    38.2 ns |
| `fe80::1ff:fe23:4567:890a%31`                   |   98.4 ns |    -       |    1423 ns |

</details>

<details class="details_class">
<summary><b> MacOS </b></summary>

| input string                                    | ipaddress | socket API | boost asio |
| ----------------------------------------------- | ---------:| ----------:| ----------:|
| `127.0.0.1`                                     |           |            |            |
| `192.168.0.1`                                   |           |            |            |
| `0.0.0.0`                                       |           |            |            |
| `100.64.0.0`                                    |           |            |            |
| `192.168.1.1`                                   |           |            |            |
| `127.239.0.1`                                   |           |            |            |
| `127.128.128.255`                               |           |            |            |
| `224.1.1.1`                                     |           |            |            |
| `2001:db8::1`                                   |           |            |            |
| `0001:0002:0003:0004:0005:0006:0007:0008%12345` |           |            |            |
| `::ffff:172.32.0.0`                             |           |            |            |
| `2002:ac1d:2d64::1`                             |           |            |            |
| `2001:0000:4136:e378:8000:63bf:3fff:fdd2`       |           |            |            |
| `2000::4136:e378:8000:63bf:3fff:fdd2`           |           |            |            |
| `2001:db8:0:0:1:0:0:1`                          |           |            |            |
| `fe80::1ff:fe23:4567:890a%31`                   |           |            |            |

</details>

<details class="details_class">
<summary><b> Windows </b></summary>

@note
Run on (16 X 3110 MHz CPU s) <br>
CPU Caches: <br>
&nbsp;&nbsp;&nbsp;&nbsp;L1 Data 48 KiB (x8) <br>
&nbsp;&nbsp;&nbsp;&nbsp;L1 Instruction 32 KiB (x8) <br>
&nbsp;&nbsp;&nbsp;&nbsp;L2 Unified 1280 KiB (x8) <br>
&nbsp;&nbsp;&nbsp;&nbsp;L3 Unified 18432 KiB (x1)

| input string                                    | ipaddress | socket API | boost asio |
| ----------------------------------------------- | ---------:| ----------:| ----------:|
| `127.0.0.1`                                     |   9.44 ns |    31.8 ns |     219 ns |
| `192.168.0.1`                                   |   11.7 ns |    34.4 ns |     227 ns |
| `0.0.0.0`                                       |   8.07 ns |    26.0 ns |     214 ns |
| `100.64.0.0`                                    |   10.9 ns |    32.0 ns |     226 ns |
| `192.168.1.1`                                   |   12.1 ns |    33.8 ns |     225 ns |
| `127.239.0.1`                                   |   12.5 ns |    34.9 ns |     227 ns |
| `127.128.128.255`                               |   16.8 ns |    40.5 ns |     235 ns |
| `224.1.1.1`                                     |   10.2 ns |    27.9 ns |     225 ns |
| `2001:db8::1`                                   |   41.8 ns |    91.1 ns |     262 ns |
| `0001:0002:0003:0004:0005:0006:0007:0008%12345` |    125 ns |    -       |     424 ns |
| `::ffff:172.32.0.0`                             |   69.4 ns |    135 ns  |     300 ns |
| `2002:ac1d:2d64::1`                             |   57.4 ns |    131 ns  |     307 ns |
| `2001:0000:4136:e378:8000:63bf:3fff:fdd2`       |    116 ns |    273 ns  |     435 ns |
| `2000::4136:e378:8000:63bf:3fff:fdd2`           |    101 ns |    214 ns  |     338 ns |
| `2001:db8:0:0:1:0:0:1`                          |   67.7 ns |    169 ns  |     321 ns |
| `fe80::1ff:fe23:4567:890a%31`                   |   71.6 ns |    -       |     349 ns |

</details>

@htmlonly

<div class="cards">

<div class="card">
  <a href="errors.html">
  <div class="card_container">
    <svg viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg"><g id="SVGRepo_bgCarrier" stroke-width="0"></g><g id="SVGRepo_tracerCarrier" stroke-linecap="round" stroke-linejoin="round"></g><g id="SVGRepo_iconCarrier"> <g id="Arrow / Arrow_Circle_Left"> <path id="Vector" d="M11 9L8 12M8 12L11 15M8 12H16M21 12C21 7.02944 16.9706 3 12 3C7.02944 3 3 7.02944 3 12C3 16.9706 7.02944 21 12 21C16.9706 21 21 16.9706 21 12Z" stroke="#000000" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"></path> </g> </g></svg>
    <h2>Error Handling</h2>
    <p>Back to error handling.</p>
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
