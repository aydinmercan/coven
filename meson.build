project('coven', ['c'],
    version: 'trunk',
    license: 'BSD-3-Clause',
    meson_version: '>=0.58',
    default_options: [
        'b_asneeded=true',
        'b_lto=true',
        'b_ndebug=if-release',
        'b_pie=true',
        'b_staticpic=true',
        'c_std=gnu17',
        'warning_level=2',
        'werror=true',
    ],
)

cc = meson.get_compiler('c')

# Default Flags
add_project_arguments(
    cc.get_supported_arguments([
        '-Walloca',
        '-Wcast-align',
        '-Wconversion',
        '-Wdate-time',
        '-Wendif-labels',
        '-Wfloat-equal',
        '-Wformat-security',
        '-Wformat=2',
        '-Wimplicit-fallthrough=5',
        '-Winit-self',
        '-Wlogical-op',
        '-Wmaybe-unitialized',
        '-Wmissing-include-dirs',
        '-Wmissing-noreturn',
        '-Wmissing-prototypes',
        '-Wnested-externs',
        '-Wno-error=#warnings',
        '-Wno-error=attribute-warning',
        '-Wno-error=unknown-attributes',
        '-Wno-error=unknown-pragmas',
        '-Wno-format-nonliteral',
        '-Wno-gnu-variable-sized-type-not-at-end',
        '-Wno-missing-braces',
        '-Wno-missing-field-initializers',
        '-Wno-string-plus-int',
        '-Wno-typedef-redefinition',
        '-Woverflow',
        '-Wredundant-decls',
        '-Wsign-conversion',
        '-Wstrict-aliasing=2',
        '-Wstrict-overflow=2',
        '-Wstrict-prototypes',
        '-Wtrampolines',
        '-Wundef',
        '-Wuninitialized',
        '-Wvla',
        '-Wwrite-strings',

        '-fcf-protection=full',
        '-fno-common',
        '-fno-strict-aliasing',
        '-fstack-protector',
        '-fstack-protector-strong',
        '-fvisibility=hidden',
        '-fwrapv',

        '-masm=intel',
    ]), language: 'c'
)

add_project_link_arguments(
    cc.get_supported_link_arguments([
        '-Wl,-z,noexecheap',
        '-Wl,-z,noexecstack',
        '-Wl,-z,now',
        '-Wl,-z,relro',
    ]), language: 'c'
)

add_project_arguments(
    '-D_DEFAULT_SOURCE',
    '-D_GNU_SOURCE',
    '-D_POSIX_C_SOURCE=200809L',
    language: 'c'
)

# Compiler-specific Behavior
if cc.get_id() == 'clang'
    add_project_link_arguments(
        cc.get_supported_link_arguments([
            '-Wl,-fuse-ld=lld'
        ]), language: 'c'
    )
elif cc.get_id() == 'gcc'
    add_project_link_arguments(
        cc.get_supported_link_arguments([
            '-Wl,-fuse-ld=gold'
        ]), language: 'c'
    )
endif

# Non-Debug Options
if get_option('buildtype') != 'debug'
    add_project_arguments(
        cc.get_supported_arguments([
            '-ffunction-sections',
            '-fdata-sections',
        ]), language: 'c'
    )

    add_project_link_arguments(
        cc.get_supported_link_arguments([
            '-Wl,--gc-sections',
        ]), language: 'c'
    )
endif

thread_dep = dependency('threads')

common_inc = include_directories('include')

common_src = []
daemon_src = []

subdir('src')

executable(
    'covend',
    [ common_src, daemon_src ],
    include_directories: [ common_inc ],
    dependencies: [
        thread_dep,
    ],
)

summary(
    {
        'Compiler': cc.get_id(),
    }, bool_yn: true
)
