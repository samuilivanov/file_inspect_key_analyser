# Testing library
enable_testing()

# function(add_doctest name lib)
#   add_executable(${name} ${name}.cpp)
#   target_compile_features(${name} PRIVATE cxx_std_17)
#   target_link_libraries(${name} PRIVATE ${lib})
# endfunction()

# add_doctest(test_bip39_ctx crypto-nano)
# add_doctest(test_lock_buffer nano)
# add_doctest(test_mnemonic crypto-nano)
# add_doctest(test_keyring crypto-nano )

# # If you register a test, then ctest and make test will run it.
# # You can also run examples and check the output, as well.
# add_test(NAME test_bip39_ctxtest COMMAND test_bip39_ctx) # Command can be a target
# add_test(NAME test_lock_buffertest COMMAND test_lock_buffer) # Command can be a target
# add_test(NAME test_mnemonic COMMAND test_mnemonic) # Command can be a target
# add_test(NAME test_keyring COMMAND test_keyring) # Command can be a target
