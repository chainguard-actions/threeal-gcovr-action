<!-- markdownlint-disable -->

# Hardening Report: threeal--gcovr-action/v1.1.0

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **threeal--gcovr-action/v1.1.0** was hardened automatically. 3 finding(s) were identified and resolved across 1 iteration(s).

## Findings Fixed

### unpinned-uses (severity: high)

Multiple workflow files use `uses:` references pinned to version tags rather than full 40-character commit SHAs, making them vulnerable to supply-chain attacks if a tag is moved or a dependency is compromised.

.github/workflows/build.yaml:
  - actions/checkout@v4.2.0 (line 13)
  - actions/setup-node@v4.0.4 (line 16)
  - threeal/setup-yarn-action@v2.0.0 (line 20)

.github/workflows/check.yaml:
  - actions/checkout@v4.2.0 (line 13)
  - actions/setup-node@v4.0.4 (line 16)
  - threeal/setup-yarn-action@v2.0.0 (line 20)

.github/workflows/test.yaml:
  - actions/checkout@v4.2.0 (line 16)
  - seanmiddleditch/gha-setup-ninja@v5 (line 19)
  - threeal/cmake-action@v2.0.0 (line 22)
  - threeal/ctest-action@v1.1.0 (line 29)
  - (and many more throughout the file)

All should be replaced with full SHA-pinned references, e.g. `actions/checkout@11bd71901bbe5b1630ceea73d27597364c9af683 # v4.2.0`.

Locations:

- `.github/workflows/build.yaml:13`
- `.github/workflows/build.yaml:16`
- `.github/workflows/build.yaml:20`
- `.github/workflows/check.yaml:13`
- `.github/workflows/check.yaml:16`
- `.github/workflows/check.yaml:20`
- `.github/workflows/test.yaml:16`
- `.github/workflows/test.yaml:19`
- `.github/workflows/test.yaml:22`
- `.github/workflows/test.yaml:29`

### missing-permissions (severity: medium)

None of the three workflow files define a top-level `permissions:` key, and no individual job within them defines a `permissions:` key either. This means all jobs run with the default repository permissions, which may be broader than necessary (e.g., `contents: write` for repositories with that default). Each workflow should declare minimal required permissions explicitly.

Locations:

- `.github/workflows/build.yaml:1`
- `.github/workflows/check.yaml:1`
- `.github/workflows/test.yaml:1`

### script-injection (severity: high)

Three `run:` blocks in test.yaml directly interpolate `${{ ... }}` expressions into the shell command string (sub-rule a). Although `steps.*.outcome` is not directly attacker-controlled, any `${{ ... }}` expression inside a `run:` block is subject to YAML template substitution before the shell processes it, making it a script-injection risk. The expressions evaluate a boolean condition and the result is passed directly to the shell as the entire command.

Offending lines:
  Line 99:  `run: ${{ steps.failed_step.outcome == 'failure' && true || false }}`
  Line 155: `run: ${{ steps.failed_branch_step.outcome == 'failure' && true || false }}`
  Line 172: `run: ${{ steps.failed_function_step.outcome == 'failure' && true || false }}`

Fix: Move the logic into a proper shell conditional, e.g.:
  `run: '[[ "${{ steps.failed_step.outcome }}" == "failure" ]]'`
or better, use an `if:` condition on the step instead.

Locations:

- `.github/workflows/test.yaml:99`
- `.github/workflows/test.yaml:155`
- `.github/workflows/test.yaml:172`

## Iteration Notes

### Iteration 1

**Fixes applied:** unpinned-uses, missing-permissions, script-injection

**Notes:**

Fixed all three findings across .github/workflows/build.yaml, check.yaml, and test.yaml:

1. unpinned-uses: Pinned all action references to full commit SHAs:
   - actions/checkout@v4.2.0 → @d632683dd7b4114ad314bca15554477dd762a938
   - actions/setup-node@v4.0.4 → @0a44ba7841725637a19e28fa30b79a866c81b0a6
   - threeal/setup-yarn-action@v2.0.0 → @ec8c075e62bc497968de40011c2b766f5e8f1ac5
   - seanmiddleditch/gha-setup-ninja@v5 → @96bed6edff20d1dd61ecff9b75cc519d516e6401
   - threeal/cmake-action@v2.0.0 → @f0425ca4df398f15fcdb5a2dd32d540fd6f22dcd
   - threeal/ctest-action@v1.1.0 → @8f90c568a3060a3cffb896e241be14bd3f53d526
   Original tags preserved as inline comments.

2. missing-permissions: Added `permissions: {}` at the top level of all three workflow files.

3. script-injection: Replaced three `run: ${{ steps.*.outcome == 'failure' && true || false }}` expressions with proper shell conditionals using env: blocks (OUTCOME: ${{ steps.*.outcome }}) and `run: '[[ "$OUTCOME" == "failure" ]]'`.

