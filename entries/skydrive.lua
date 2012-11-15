signin = "https://signup.live.com/signup.aspx"
description = "Microsoft SkyDrive"

dir = function(params)
    local files = {}
    local i = 10
    for k, v in pairs(params) do
        i = i + 1
        files[i] = {attribute = 'file', filename = k .. '-' .. v, size = 1, create = '2012-12-30 15:03:54', access = '2012-12-31 15:03:54', write = '2012-12-31 10:03:54'}
    end
    files[i + 1] = {attribute = 'file', filename = 'a.txt', size = i, create = '2012-12-30 15:03:54', access = '2012-12-31 15:03:54', write = '2012-12-31 10:03:54'}
    files[i + 2] = {attribute = 'directory', filename = params.username, create = '5122445'}
    return files
end
