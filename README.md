# Thale
[![Github Workflow Status](https://img.shields.io/github/actions/workflow/status/PeterGriffinSr/ThaleC/ci.yml)](https://github.com/PeterGriffinSr/ThaleC/actions)
[![GitHub license](https://img.shields.io/github/license/PeterGriffinSr/ThaleC)](https://github.com/PeterGriffinSr/ThaleC/blob/main/LICENSE)

---

**Thale** is a small, purely functional programming language with immutability, expression-based syntax, and powerful pattern matching. The language emphasizes clarity, safety, and functional purity.

---

## Getting Started
Create a new Thale script with the `.thl` extension. Here's a basic example:
```thl
effect Console { print : String -> () }

main : Effect ()
main -> Console.print "Hello, world!"
```
To build and run your script:
`thale run Main.thl`

---

## Contributing
We welcome contributions! Whether it's a bug report, feature suggestion, or code contribution:

- File issues via the [issue tracker](http://github.com/PeterGriffinSr/ThaleC/issues)
- For the repo and open a pull request

Please follow [conventional commit and PR practices where possible](CONTRIBUTING.md).

---

## License

Thale is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.