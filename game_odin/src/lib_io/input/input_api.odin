package input


InputStatus :: enum
{
    Void = 0,
    OK,
    Error
}


InputArray :: struct
{
    _id_prev: u8,
    _id_curr: u8,

    _inputs: [2]Input,

    status: InputStatus,
}

@(private)
reset_input_array :: proc(inputs: ^InputArray)
{
    inputs._id_curr = 0
    inputs._id_prev = 1

    for &input in inputs._inputs
    {
        reset_input_state(&input)
    }

    inputs.status = .Void
}


init :: proc(inputs: ^InputArray) -> bool
{
    if (inputs.status != .Void)
    {
        return false
    }

    reset_input_array(inputs)

    ok := api_init(inputs)

    inputs.status = ok ? .OK : .Error

    return ok
}


close :: proc(inputs: ^InputArray)
{
    api_close()
    reset_input_array(inputs)
}


record_input :: proc(inputs: ^InputArray)
{
    prev := prev(inputs)^
    curr := curr(inputs)

    copy_input_state(prev, curr)

    api_record_input(inputs)
}


prev :: proc(inputs: ^InputArray) -> ^Input
{
    return &inputs._inputs[inputs._id_prev]
}


curr :: proc(inputs: ^InputArray) -> ^Input
{
    return &inputs._inputs[inputs._id_curr]
}


get :: proc(inputs: ^InputArray) -> Input
{
    return inputs._inputs[inputs._id_curr]
}


swap :: proc(inputs: ^InputArray)
{
    c := inputs._id_curr
    inputs._id_curr = inputs._id_prev
    inputs._id_prev = c
}
