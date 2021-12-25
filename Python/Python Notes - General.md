# Python Notes - General

[toc]

## Best practices for Python Main functions

1. Put most code into a function or class
2. Use __name__ to control execution of the code
3. Create a function called main() to contain the code we want to run
4. Call other functions from main()

```python
if __name__ == "__main__":
    # execute
    main()
```

can execute from command line; and can be imported without being executed

