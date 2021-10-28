<div id="top"></div>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]



<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/prestonbridgers/ntd">
    <img src="resources/logo.png" alt="Logo" width="80" height="80">
  </a>

<h3 align="center">nCurses ToDo</h3>

  <p align="center">
    A simple, lightweight todo list creation/management application
    <br />
    <a href="https://github.com/prestonbridgers/ntd"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/prestonbridgers/ntd/issues">Report Bug</a>
    ·
    <a href="https://github.com/prestonbridgers/ntd/issues">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li>
      <a href="#usage">Usage</a>
      <ul>
        <li><a href="#keybinds">Keybinds</a></li>
      </ul>
    </li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

[![Product Name Screen Shot][product-screenshot]](https://example.com)

`ntd` is a simple, lightweight todo list creation/management application.
It boasts a very clean, minimal terminal user interface (TUI).
The program is written in C and uses the nCurses library to handle the TUI.

<p align="right">(<a href="#top">back to top</a>)</p>



### Built With

* [nCurses](https://invisible-island.net/ncurses/ncurses.faq.html)

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running follow these simple example steps.

### Prerequisites

* nCurses
* gcc
* cMake

These are very common packages, and can be easily googled to find the command
to install them on your distribution of linux.

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/prestonbridgers/ntd.git
   cd ntd
   ```
1. Create obj directory
   ```sh
   mkdir obj
   ```
1. Compile the program
   ```sh
   make
   ```

The compiled binary can be found in the `bin` folder.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

Simply run the `make run` command when in the root directory of the project or
create a symbolic link from a directory in your `PATH` to the compiled binary
found in `bin`.

The program currently supports the following features:

- Loading/saving a todo list from/to a file.
- Adding/deleting entries
- Marking entries as complete/incomplete

Running the program automatically creates an `entries.txt` file
in the current directory. On exit, the list is saved to that same file.

To add/delete/mark an entry, press the appropriate key, type the number
assigned to the entry, and press Enter.

An attempt to delete or mark an entry number that doesn't exist, nothing will
happen and you will be returned to the delete/mark window will close.

### Keybinds

```
q -> Quits the program
a -> Opens the add entry menu
d -> Opens the delete entry menu
m -> Opens the mark entry menu
```

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- ROADMAP -->
## Roadmap

- [] Allow the user to specify file in which to save the list
- [] Add date field to the entries
- [] Add support for multiple todo lists
    - [] Add menu for selecting todo list on startup and keypress (maybe 't')
    - [] Add a list of directories in which to look for todo lists

See the [open issues](https://github.com/prestonbridgers/ntd/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Preston Bridgers - prestonbridgers@gmail.com@.com

Project Link: [https://github.com/prestonbridgers/ntd](https://github.com/prestonbridgers/ntd)

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/prestonbridgers/ntd.svg?style=for-the-badge
[contributors-url]: https://github.com/prestonbridgers/ntd/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/prestonbridgers/ntd.svg?style=for-the-badge
[forks-url]: https://github.com/prestonbridgers/ntd/network/members
[stars-shield]: https://img.shields.io/github/stars/prestonbridgers/ntd.svg?style=for-the-badge
[stars-url]: https://github.com/prestonbridgers/ntd/stargazers
[issues-shield]: https://img.shields.io/github/issues/prestonbridgers/ntd.svg?style=for-the-badge
[issues-url]: https://github.com/prestonbridgers/ntd/issues
[license-shield]: https://img.shields.io/github/license/prestonbridgers/ntd.svg?style=for-the-badge
[license-url]: https://github.com/prestonbridgers/ntd/blob/master/LICENSE.txt
[product-screenshot]: resources/ntd_main_window.png
